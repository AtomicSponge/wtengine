/*!
 * \brief WTEngine | File:  wte_main.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details The main engine object.
 */

#ifndef WTE_MAIN_HPP
#define WTE_MAIN_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_memfile.h>

#include <ctime>
#include <string>
#include <map>
#include <stdexcept>

#include "wte_global_defines.hpp"
#include "engine_cfg_map.hpp"
#include "game_cfg_map.hpp"
#include "engine_flags.hpp"
#include "alert.hpp"
#include "mgr/engine_time.hpp"
#include "mgr/managers.hpp"

namespace wte
{

//! WTEngine Main Class.
/*!
  The main engine object.
  Sets up various system objects used by the engine.
  Contains the main game loop and members for managing the game and engine.
*/
class wte_main {
    public:
        //!  Frees up instance, set initialized flag to false.
        //!  Also makes sure to unload the engine.
        inline ~wte_main() {
            if(load_called == true) wte_unload();

            al_destroy_timer(main_timer);
            al_destroy_event_queue(main_queue);
            al_destroy_display(display);
            al_uninstall_system();

            initialized = false;
        };

        //!  Remove copy constructor.
        wte_main(const wte_main&) = delete;
        //!  Remove assignment operator.
        void operator=(wte_main const&) = delete;

        //!  Call first to load the engine.
        inline void wte_load(void) {
            //  Start the input & audio threads.
            input_th.start();

            //  Ignore Audio Manager if WTE_NO_AUDIO build flag is defined.
            #ifndef WTE_NO_AUDIO
            audio_th.start();
            #endif

            //  Initialize renderer and menu manager.
            screen.initialize(al_create_builtin_font());
            menus.initialize(al_create_builtin_font(), WTE_COLOR_WHITE, WTE_COLOR_DARKPURPLE);

            //  Load user configured menus.
            load_menus();

            //  Init done, set flag to true.
            load_called = true;
        };

        //!  Call to unload the engine.
        inline void wte_unload(void) {
            input_th.stop();

            //  Ignore Audio Manager if WTE_NO_AUDIO build flag is defined.
            #ifndef WTE_NO_AUDIO
            audio_th.stop();
            #endif

            load_called = false;
        };
    
        //!  Call to start up the main game loop.
        void do_game(void);

    protected:
        //!  Force single instance, set initialized flag to true.
        //!  Throws a runtime error if another instance is called.
        inline wte_main() : load_called(false) {
            if(initialized == true) throw std::runtime_error("WTEngine already running!");
            initialized = true;

            //  Initialize Allegro.
            if(!al_init()) throw std::runtime_error("Allegro failed to load!");

            //  Initialize additional Allegro components.
            if(!al_install_keyboard()) throw std::runtime_error("Failed to detect keyboard!");
            if(!al_init_image_addon()) throw std::runtime_error("Failed to load Allegro image addon!");
            if(!al_init_font_addon()) throw std::runtime_error("Failed to load Allegro font addon!");
            al_install_joystick();

            if(!engine_cfg::is_reg("screen_width")) throw std::runtime_error("Screen width not set!");
            if(!engine_cfg::is_reg("screen_height")) throw std::runtime_error("Screen height not set!");
            display = al_create_display(engine_cfg::get<int>("screen_width"),
                                        engine_cfg::get<int>("screen_height"));
            if(!display) throw std::runtime_error("Failed to configure display!");

            main_timer = al_create_timer(1.0 / WTE_TICKS_PER_SECOND);
            if(!main_timer) throw std::runtime_error("Failed to create timer!");

            main_queue = al_create_event_queue();
            if(!main_queue) throw std::runtime_error("Failed to create event queue!");

            //  Register event sources.
            al_register_event_source(main_queue, al_get_display_event_source(display));
            al_register_event_source(main_queue, al_get_timer_event_source(main_timer));

            //  Map commands to enums for switching over in the system msg handler.
            map_cmd_str_values["exit"] = CMD_STR_EXIT;
            map_cmd_str_values["alert"] = CMD_STR_ALERT;
            map_cmd_str_values["new_game"] = CMD_STR_NEW_GAME;
            map_cmd_str_values["end_game"] = CMD_STR_END_GAME;
            map_cmd_str_values["open_menu"] = CMD_STR_OPEN_MENU;
            map_cmd_str_values["close_menu"] = CMD_STR_CLOSE_MENU;
            map_cmd_str_values["enable_system"] = CMD_STR_ENABLE_SYSTEM;
            map_cmd_str_values["disable_system"] = CMD_STR_DISABLE_SYSTEM;
            map_cmd_str_values["set_engcfg"] = CMD_STR_SET_ENGCFG;
            map_cmd_str_values["set_gamecfg"] = CMD_STR_SET_GAMECFG;

            //  Set default colors for alerts.
            alert::set_font_color(WTE_COLOR_WHITE);
            alert::set_bg_color(WTE_COLOR_RED);
        };
    
        /* These functions are defined in their own source file */
        //!  Define this to implement custom menu layout.
        virtual void load_menus(void) = 0;
        //!  Define this to load all systems to be used by the game.
        virtual void load_systems(void) = 0;
        //!  Define what gets loaded when a game starts.
        virtual void load_game(void) = 0;
        //!  Define what happens at the end of a game.
        virtual void end_game(void) = 0;
        //!  Optional:  On menu open.
        virtual void on_menu_open(void) {};
        //!  Optional:  On menu open.
        virtual void on_menu_close(void) {};
        //!  Optional:  Define custom system message handling.
        virtual void handle_custom_sys_msg(message) {};
        /* *** End custom members *** */

        mgr::message_manager messages;
        mgr::render_manager screen;
        mgr::entity_manager world;
        mgr::system_manager systems;
        mgr::menu_manager menus;
        mgr::input_manager input_th;

        //  Ignore Audio Manager if WTE_NO_AUDIO build flag is defined.
        #ifndef WTE_NO_AUDIO
        mgr::audio_manager audio_th;
        #endif

    private:
        //!  Start up a new game.
        void generate_new_game(void);
        //!  End the game in progress.
        void unload_game(void);
        //!  Process messages passed to the system.
        void handle_sys_msg(message_container);

        //  Used for switching on system messages:
        enum CMD_STR_VALUE {
            CMD_STR_EXIT,           CMD_STR_ALERT,
            CMD_STR_NEW_GAME,       CMD_STR_END_GAME,
            CMD_STR_OPEN_MENU,      CMD_STR_CLOSE_MENU,
            CMD_STR_ENABLE_SYSTEM,  CMD_STR_DISABLE_SYSTEM,
            CMD_STR_SET_ENGCFG,     CMD_STR_SET_GAMECFG
        };
        std::map<std::string, CMD_STR_VALUE> map_cmd_str_values;

        inline static ALLEGRO_DISPLAY* display = NULL;
        inline static ALLEGRO_TIMER* main_timer = NULL;
        inline static ALLEGRO_EVENT_QUEUE* main_queue = NULL;
        ALLEGRO_EVENT event;

        bool load_called;        //  Flag to make sure wte_load was called.

        inline static bool initialized = false; //  Restrict to one instance of the engine running.
};

/*!
  Call every time a new game is starting.
*/
inline void wte_main::generate_new_game(void) {
    std::srand(std::time(nullptr));  //  Seed random.

    //  Set global flags.
    engine_flags::unset(GAME_MENU_OPENED);
    engine_flags::set(GAME_STARTED);

    //  Clear world and load starting entities.
    world.clear();
    load_game();

    //  Load systems and prevent further systems from being loaded.
    load_systems();
    systems.finalize();

    //  Load a new message data file.
    messages.new_data_file("data\\game.sdf");  //  Update later to load from settings.

    //  Restart the timer at zero.
    al_stop_timer(main_timer);
    al_set_timer_count(main_timer, 0);
    al_start_timer(main_timer);
}

/*!
  Calls the user defined end game process, then shuts down the game.
*/
inline void wte_main::unload_game(void) {
    al_stop_timer(main_timer);
    end_game();

    world.clear();
    systems.clear();

    engine_flags::unset(GAME_STARTED);
    engine_flags::set(GAME_MENU_OPENED);
}

/*!
  The main game loop.
*/
inline void wte_main::do_game(void) {
    if(load_called == false) wte_load();  //  Auto call load.

    bool queue_not_empty = false;
    message_container temp_msgs;

    messages.clear_queue();

    engine_flags::set(IS_RUNNING);
    engine_flags::unset(GAME_STARTED);
    engine_flags::set(GAME_MENU_OPENED);

    //  test code.
    engine_flags::unset(WAIT_FOR_VSYNC);
    engine_flags::set(DRAW_HITBOX);
    engine_flags::set(DRAW_FPS);
    //  end test code.

    while(engine_flags::is_set(IS_RUNNING)) {
        //  Pause / resume timer depending on if the game menu is opened.
        //  Also process the on_menu events.
        if(engine_flags::is_set(GAME_MENU_OPENED) && al_get_timer_started(main_timer)) {
            al_stop_timer(main_timer);
            on_menu_open();
        }
        if(!engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(main_timer)) {
            on_menu_close();
            al_resume_timer(main_timer);
        }

        //  Game not running, make sure the timer isn't and force the menu manager.
        if(!engine_flags::is_set(GAME_STARTED)) {
            al_stop_timer(main_timer);
            engine_flags::set(GAME_MENU_OPENED);
        }
        //  Game menu is opened, run the menu manager
        if(engine_flags::is_set(GAME_MENU_OPENED)) menus.run(messages);

        /* *** GAME LOOP ************************************************************ */
        //  Capture event from queue.
        queue_not_empty = al_get_next_event(main_queue, &event);
        //  Call our game logic update on timer events.  Timer is only running when the game is running.
        if(event.type == ALLEGRO_EVENT_TIMER && queue_not_empty) {
            //  Set the engine_time object to the current time.
            mgr::engine_time::set_time(al_get_timer_count(main_timer));
            //  Run all systems.
            systems.run(world, messages, al_get_timer_count(main_timer));
            //  Process messages.
            systems.dispatch(world, messages);
        }
        /* *** END GAME LOOP ******************************************************** */

        //  Render the screen.
        screen.render(menus, world);

        //  Ignore Audio Manager if WTE_NO_AUDIO build flag is defined.
        #ifndef WTE_NO_AUDIO
        //  Send audio messages to the audio queue.
        temp_msgs = messages.get_messages("audio");
        if(!temp_msgs.empty()) audio_th.transfer_messages(temp_msgs);
        #endif

        //  Get any system messages and pass to handler.
        temp_msgs = messages.get_messages("system");
        if(!temp_msgs.empty()) handle_sys_msg(temp_msgs);

        //  Ignore message pruning if WTE_NO_PRUNE build flag is defined.
        #ifndef WTE_NO_PRUNE
        //  Delete messages that were not processed.
        messages.prune();
        #endif

        //  Force quit if the game window is closed.
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) engine_flags::unset(IS_RUNNING);
    }
}

/*!
  Switch over the system messages and process.
  Remaining messages are passed to the custom handler.
*/
inline void wte_main::handle_sys_msg(message_container sys_msgs) {
    for(auto it = sys_msgs.begin(); it != sys_msgs.end();) {
        //  Switch over the system messages, deleting each as they are processed.
        switch(map_cmd_str_values[it->get_cmd()]) {
            //  cmd:  exit - Shut down engine.
            case CMD_STR_EXIT:
                if(engine_flags::is_set(GAME_STARTED)) unload_game();
                engine_flags::unset(IS_RUNNING);
                it = sys_msgs.erase(it);
                break;

            //  cmd:  alert - Display an alert.
            case CMD_STR_ALERT:
                alert::set_alert(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  new_game - start up a new game.
            case CMD_STR_NEW_GAME:
                if(engine_flags::is_set(GAME_STARTED)) unload_game();
                menus.reset();
                generate_new_game();
                it = sys_msgs.erase(it);
                break;

            //  cmd:  end_game - end current game.
            case CMD_STR_END_GAME:
                menus.reset();
                unload_game();
                it = sys_msgs.erase(it);
                break;

            //  cmd:  open_menu argstring - open a menu, passing a string as an argument.
            //  If the menu doesn't exist, the default will be opened.
            case CMD_STR_OPEN_MENU:
                engine_flags::set(GAME_MENU_OPENED);
                menus.open_menu(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  close_menu argstring - close the opened menu.
            //  If argstring = "all", close all opened menus.
            case CMD_STR_CLOSE_MENU:
                if(it->get_arg(0) == "all") menus.reset();
                else menus.close_menu();
                it = sys_msgs.erase(it);
                break;

            //  cmd:  enable_system - Turn a system on for processing.
            case CMD_STR_ENABLE_SYSTEM:
                systems.enable_system(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  disable_system - Turn a system off so it's run member is skipped.
            //  Message dispatching is still processed.
            case CMD_STR_DISABLE_SYSTEM:
                systems.disable_system(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  set_engcfg - Set engine cfg variable.
            case CMD_STR_SET_ENGCFG:
                for(std::size_t i = 0; i < it->num_args(); i++)
                    engine_cfg::set(it->get_arg(i));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  set_gamecfg - Set game cfg variable.
            case CMD_STR_SET_GAMECFG:
                for(std::size_t i = 0; i < it->num_args(); i++)
                    game_cfg::set(it->get_arg(i));
                it = sys_msgs.erase(it);
                break;

            //  cmd:  new_cmd - description
            //case CMD_STR_X:
                //
                //it = sys_msgs.erase(it);
                //break;

            //  Command not defined, iterate to next
            default:
                it++;
        }
    }

    //  Pass remaining system messages to the custom handler.
    if(!sys_msgs.empty())
        for(auto it = sys_msgs.begin(); it != sys_msgs.end(); it++)
            handle_custom_sys_msg(*it);
}

} //  end namespace wte

#endif
