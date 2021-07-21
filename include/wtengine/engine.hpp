/*!
 * WTEngine | File:  engine.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ENGINE_HPP
#define WTE_ENGINE_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "wtengine/config.hpp"
#include "wtengine/display.hpp"
#include "wtengine/input.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/commands.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/mgr/_managers.hpp"

namespace wte
{

/*!
 * \class engine
 * \brief The main engine object.
 * 
 * Sets up various system objects used by the engine.
 * Contains the main game loop and members for managing the game and engine.
 */
class engine : private display, public input, public config {
    public:
        /*!
         * \brief Engine destructor.
         * 
         * Frees up instance, set initialized flag to false.
         * Also makes sure to unload the engine.
         */
        inline ~engine() {
            PHYSFS_deinit();

            al_destroy_timer(main_timer);
            al_destroy_event_queue(main_event_queue);
            destroy_input_event_queue();
            destroy_display();
            al_inhibit_screensaver(false);
            al_uninstall_system();

            initialized = false;
        };

        //!  Remove copy constructor.
        inline engine(const engine&) = delete;
        //!  Remove assignment operator.
        inline void operator=(engine const&) = delete;

        /*!
         * \brief Add file path to provide to PhysFS.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param flocation File location to add to PhysFS.
         */
        inline static void add_file_location(const std::string& flocation) {
            file_locations.push_back(flocation);
        };

        void do_game(void);

    protected:
        /*!
         * \brief Engine constructor.
         * 
         * Force single instance, set initialized flag to true.
         * Throws a runtime error if another instance is called.
         * 
         * \param argc Command line arguments.
         * \param argv Command line arguments count.
         * \param title Window title.
         */
        inline engine(const int& argc, char** const& argv, const std::string& title) : display(title) {
            if(initialized == true) throw std::runtime_error(get_window_title() + " already running!");
            initialized = true;

            //  Initialize Allegro.
            if(!al_init()) throw std::runtime_error("Allegro failed to load!");

            //  Initialize additional Allegro components.
            if(!al_install_keyboard()) throw std::runtime_error("Failed to detect keyboard!");
            if(!al_init_image_addon()) throw std::runtime_error("Failed to load Allegro image addon!");
            if(!al_init_font_addon()) throw std::runtime_error("Failed to load Allegro font addon!");
            if(!al_install_audio()) throw std::runtime_error("Failed to load audio!");
            if(!al_init_acodec_addon()) throw std::runtime_error("Failed to load Allegro audio addon!");
            al_install_joystick();

            //  Configure PhysFS.
            if(!PHYSFS_init(argv[0])) throw std::runtime_error("Failed to load PhysFS!");
            if(file_locations.empty()) throw std::runtime_error("Need to configure locations for PhysFS!");
            for(auto & it : file_locations) PHYSFS_mount(it.c_str(), NULL, 1);
            al_set_physfs_file_interface();

            //  Configure display.  Called from wte_display class.
            create_display();

            //  Disable pesky screensavers.
            al_inhibit_screensaver(true);

            //  Configure main timer.
            main_timer = al_create_timer(1.0f / WTE_TICKS_PER_SECOND);
            if(!main_timer) throw std::runtime_error("Failed to create timer!");

            //  Configure main event queue.
            main_event_queue = al_create_event_queue();
            if(!main_event_queue) throw std::runtime_error("Failed to create main event queue!");

            //  Register event sources.
            al_register_event_source(main_event_queue, al_get_display_event_source(_display));
            al_register_event_source(main_event_queue, al_get_timer_event_source(main_timer));

            create_input_event_queue();

            //  commands
            cmds.add("exit", [this](const msg_arg_list& args) {
                if(config::flags::game_started) process_end_game();
                config::flags::is_running = false;
            });
            cmds.add("alert", [this](const msg_arg_list& args) {
                alert::set(args[0]);
            });
            cmds.add("new_game", [this](const msg_arg_list& args) {
                if(!config::flags::game_started) {
                    mgr::menus::reset();
                    process_new_game(args[0]);
                }
            });
            cmds.add("end_game", [this](const msg_arg_list& args) {
                if(config::flags::game_started) {
                    process_end_game();
                    mgr::menus::reset();
                }
            });
            cmds.add("open_menu", [this](const msg_arg_list& args) {
                mgr::menus::open_menu(args[0]);
            });
            cmds.add("close_menu", [this](const msg_arg_list& args) {
                if(args[0] == "all") mgr::menus::reset();
                else mgr::menus::close_menu();
            });
            cmds.add("reconf_display", [this](const msg_arg_list& args) {
                const bool timer_running = al_get_timer_started(main_timer);
                //  Make sure the timer isn't running and unregister the display.
                if(timer_running) al_stop_timer(main_timer);
                al_pause_event_queue(main_event_queue, true);
                al_unregister_event_source(main_event_queue, al_get_display_event_source(_display));
                //  Backup temp bitmaps.
                mgr_inf.bitmap_backup();
                //  Reload the display.
                reconf_display();
                //  Reload any temp bitmaps.
                mgr_inf.bitmap_reload();
                //  Register display event source and resume timer if it was running.
                al_register_event_source(main_event_queue, al_get_display_event_source(_display));
                al_pause_event_queue(main_event_queue, false);
                if(timer_running) al_resume_timer(main_timer);
            });
            cmds.add("fps_counter", [this](const msg_arg_list& args) {
                if(args[0] == "on") config::flags::draw_fps = true;
                if(args[0] == "off") config::flags::draw_fps = false;
            });
            cmds.add("load_script", [this](const msg_arg_list& args) {
                if(config::flags::game_started && args[0] != "") {
                    if(!mgr::messages::load_script(args[0]))
                        alert::set("Error loading script:  " + args[0]);
                }
            });

            //  Set default colors for alerts.
            alert::set_font_color(WTE_COLOR_WHITE);
            alert::set_bg_color(WTE_COLOR_RED);
        };

        /* These function members are overridden in the derived class */
        //!  Define this to implement custom menu layout.
        virtual void load_menus(void) = 0;
        //!  Define this to load all systems to be used by the game.
        virtual void load_systems(void) = 0;
        //!  Define what gets loaded when a game starts.
        virtual void new_game(void) = 0;
        //!  Define what happens at the end of a game.
        virtual void end_game(void) = 0;
        //!  Optional:  On menu open.
        virtual void on_menu_open(void) {};
        //!  Optional:  On menu close.
        virtual void on_menu_close(void) {};
        //!  Optional:  Out of focus handler
        virtual void out_of_focus(void) {};
        //!  Optional:  In focus handler
        virtual void back_in_focus(void) {};
        /* *** End overridden function members *** */
    private:
        /*!
         * \brief Load the engine's managers.
         * 
         * Called before the main loop starts.
         */
        inline void wte_load(void) {
            //  Initialize managers that require it.
            mgr_inf.renderer_init();
            mgr_inf.audio_init();
            mgr_inf.menus_init();

            //  Load user configured menus.
            load_menus();
        };

        /*!
         * \brief Unload the engine's managers.
         * 
         * Called after the main loop ends running.
         */
        inline void wte_unload(void) {
            mgr_inf.menus_de_init();
            mgr_inf.audio_de_init();
            mgr_inf.renderer_de_init();
        };

        void process_new_game(const std::string&);
        void process_end_game(void);
        void handle_sys_msg(message_container&);

        //  Interface for manager private member access.
        mgr::interface mgr_inf;
        //  Commands.
        inline static commands cmds;

        //  Allegro objects used by the engine.
        inline static ALLEGRO_TIMER* main_timer = NULL;
        inline static ALLEGRO_EVENT_QUEUE* main_event_queue = NULL;

        //  Vector of file paths to provide to PhysFS.
        inline static std::vector<std::string> file_locations = {};
        //  Restrict to one instance of the engine running.
        inline static bool initialized = false;
};

/*!
 * \brief Call to start a new game.
 * 
 * Loads a game data file and user defined systems and starting entities.
 * 
 * \param game_data Game data file to load.
 */
inline void engine::process_new_game(const std::string& game_data) {
    std::srand(std::time(nullptr));  //  Seed random, using time.

    //  Make sure the menu isn't opened.
    config::flags::game_menu_opened = false;

    //  Load a new message data file.
    if(!game_data.empty()) mgr_inf.messages_load_file(game_data);

    //  Load systems and prevent further systems from being loaded.
    load_systems();
    mgr_inf.systems_finalize();
    if(mgr_inf.systems_empty()) throw std::runtime_error("No systems have been loaded!");

    //  Stop audio manager from playing sounds.
    mgr::audio::music_stop();
    mgr::audio::ambiance_stop();
    mgr::audio::voice_stop();
    mgr::audio::sample_unload("all");
    mgr::audio::sample_clear_instances();
    
    //  Clear world and load starting entities.
    mgr::entities::clear();
    new_game();

    //  Restart the timer at zero.
    al_stop_timer(main_timer);
    al_set_timer_count(main_timer, 0);
    engine_time::set_time(al_get_timer_count(main_timer));
    config::flags::game_started = true;
    config::flags::input_enabled = true;
    al_start_timer(main_timer);
};

/*!
 * \brief Call to end the game.
 * 
 * Clears out the entities and systems and runs user defined end process.
 */
inline void engine::process_end_game(void) {
    al_stop_timer(main_timer);
    config::flags::game_started = false;
    config::flags::input_enabled = true;
    al_set_timer_count(main_timer, 0);
    engine_time::set_time(al_get_timer_count(main_timer));

    //  Stop audio manager from playing sounds.
    mgr::audio::music_stop();
    mgr::audio::ambiance_stop();
    mgr::audio::voice_stop();
    mgr::audio::sample_unload("all");
    mgr::audio::sample_clear_instances();

    //  Call end game process.
    end_game();

    //  Clear world and systems.
    mgr::entities::clear();
    mgr_inf.systems_clear();

    //  Open the menus.
    config::flags::game_menu_opened = true;
};

/*!
 * \brief The main engine loop.
 */
inline void engine::do_game(void) {
    wte_load();

    //  Set default states.
    config::flags::is_running = true;
    config::flags::game_started = false;
    config::flags::game_menu_opened = true;

    /* *** ENGINE LOOP ************************************************************ */
    while(config::flags::is_running) {
        if(!config::flags::game_started) {            //  Game not running.
            al_stop_timer(main_timer);                //  Make sure the timer isn't.
            config::flags::game_menu_opened = true;   //  And force the menu manager.
        }

        //  Check for input.
        check_input_events();

        //  Pause / resume timer depending on if the game menu is opened.
        //  Also process the on_menu events.
        if(config::flags::game_menu_opened && al_get_timer_started(main_timer)) {
            al_stop_timer(main_timer);
            on_menu_open();
        }
        if(!config::flags::game_menu_opened && !al_get_timer_started(main_timer)) {
            on_menu_close();
            al_resume_timer(main_timer);
        }

        //  Game menu is opened, run the menu manager.
        if(config::flags::game_menu_opened) mgr_inf.menus_run();

        /* *** GAME LOOP ************************************************************ */
        ALLEGRO_EVENT event;
        const bool queue_not_empty = al_get_next_event(main_event_queue, &event);
        //  Call our game logic update on timer events.  Timer is only running when the game is running.
        if(queue_not_empty && event.type == ALLEGRO_EVENT_TIMER) {
            //  Set the engine_time object to the current time.
            engine_time::set_time(al_get_timer_count(main_timer));

            //  Run all mgr::systems::
            mgr_inf.systems_run();
            //  Process mgr::messages::
            mgr_inf.systems_dispatch();

            //  Get any spawner messages and pass to handler.
            mgr_inf.spawner_process_messages(mgr_inf.messages_get("spawner"));
        }
        /* *** END GAME LOOP ******************************************************** */

        //  Render the screen.
        mgr_inf.renderer_run();

        //  Get any system messages and pass to handler.
        cmds.process_messages(mgr_inf.messages_get("system"));
        //  Send audio messages to the audio queue.
        mgr_inf.audio_process_messages(mgr_inf.messages_get("audio"));
        //  Delete timed messages that were not processed.
        mgr_inf.messages_prune();

        //  Check if display looses focus.
        if(event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_OUT) {
            out_of_focus();
        }

        //  Check if display returns to focus.
        if(event.type == ALLEGRO_EVENT_DISPLAY_SWITCH_IN) {
            back_in_focus();
        }

        //  Force quit if the game window is closed.
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            if(config::flags::game_started) process_end_game();
            config::flags::is_running = false;
        }
    }
    /* *** END ENGINE LOOP ******************************************************** */

    wte_unload();
};

} //  end namespace wte

#endif
