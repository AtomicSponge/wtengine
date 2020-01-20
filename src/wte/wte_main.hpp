/*
  WTEngine
  By:  Matthew Evans
  File:  wte_main.hpp

  See LICENSE.txt for copyright information

  The main engine class.  Extend this to create your game loop.
*/

#ifndef WTE_MAIN_HPP
#define WTE_MAIN_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>
//#include <allegro5/allegro_memfile.h>

#include <ctime>
#include <string>
#include <map>
#include <stdexcept>

#include "wte_globals.hpp"
#include "menu_manager.hpp"
#include "entity_manager.hpp"
#include "system_manager.hpp"
#include "audio_manager.hpp"
#include "message_queue.hpp"
#include "renderer.hpp"
#include "get_input.hpp"

namespace wte
{

//! wte_main class
/*!
  Main engine object
*/
class wte_main {
    public:
        virtual ~wte_main();

        wte_main(const wte_main&) = delete;
        void operator=(wte_main const&) = delete;

        void wte_init(void);                    /*!< Initialize the engine */
        void wte_unload(void);                  /*!< Unload the engine */
        void do_game(void);                     /*!< Run the game loop */

    private:
        void handle_sys_msg(msg::message_container);
        void generate_new_game(void);    /*!< Call to generate a new game */
        void unload_game(void);

        ALLEGRO_DISPLAY *display;               /*!< Display to draw to */
        ALLEGRO_TIMER *main_timer;              /*!< Timer to control game loop */
        ALLEGRO_EVENT_QUEUE *main_queue;        /*!< Main event queue */
        ALLEGRO_EVENT event;                    /*!< Container to fetch event */

        msg::message_queue messages;            /*!< Message queue */
        renderer game_screen;                   /*!< The renderer used to draw the game environment */

        //  Used for switching on system messages:
        enum cmd_str_value { ev_cmd_ndef,
                             ev_cmd1,
                             ev_cmd2,
                             ev_cmd3,
                             ev_end };
        std::map<std::string, cmd_str_value> map_cmd_str_values;

        bool init_called;                       /*!< Flag to make sure wte_init was called */

        static bool initialized;                /*!< Restrict to one instance of the engine running */

    protected:
        wte_main();

        virtual void load_menus(void) = 0;      /*!< Override to load custom menus */
        virtual void load_systems(void) = 0;    /*!< Override to load custom systems */
        virtual void load_game(void) = 0;       /*!< Override to load initial entities */
        virtual void end_game(void) = 0;        /*!< Override to define end game process */

        virtual void handle_custom_sys_msg(msg::message) {};

        ecs::entity_manager world;              /*!< Manager for entities */
        ecs::system_manager systems;            /*!< Manager for systems */
        mnu::menu_manager menus;                /*!< Manager for menus */
};

inline bool wte_main::initialized = false;

//! wte_main constructor
/*!
  Verify WTEngine isn't already running and set the init flag to false
*/
inline wte_main::wte_main() {
    if(initialized == true) throw std::runtime_error("WTEngine already running!");
    initialized = true;

    init_called = false;
}

//! wte_main destructor
/*!
  Call wte_unload if it wasn't manually called
*/
inline wte_main::~wte_main() {
    if(init_called == true) wte_unload();

    initialized = false;
}

//! Initialize WTEngine
/*!
  Register everything for the engine to run
*/
inline void wte_main::wte_init(void) {
    //  Initialize Allegro and it's various objects
    if(!al_init()) throw std::runtime_error("Allegro failed to load!"); //  Allegro didn't load - Exit

    display = al_create_display(screen_width, screen_height);
    if(!display) throw std::runtime_error("Failed to configure display!"); //  Failed to set display - Exit

    main_timer = al_create_timer(1.0 / TICKS_PER_SECOND);
    if(!main_timer) throw std::runtime_error("Failed to create timer!"); //  Failed to create timer - Exit

    main_queue = al_create_event_queue();
    if(!main_queue) throw std::runtime_error("Failed to create event queue!"); //  Failed to create event queue - Exit

    //  Initialize additional Allegro components
    al_install_keyboard();
    al_install_joystick();
    al_init_image_addon();
    al_init_font_addon();
    al_install_audio();
    //al_init_acodec_addon();

    //  Register event sources
    al_register_event_source(main_queue, al_get_display_event_source(display));
    al_register_event_source(main_queue, al_get_timer_event_source(main_timer));

    //  Start the input & audio threads as detached
    al_run_detached_thread(get_input, NULL);
    al_run_detached_thread(audio_manager, NULL);

    //  Initialize renderer and menu manager
    game_screen.initialize(al_create_builtin_font());
    menus.initialize(al_create_builtin_font(), WTE_COLOR_WHITE);

    //  Load user configured menus & systems
    load_menus();
    load_systems();
    //  Prevent further systems from being loaded
    systems.finalize();

    //  Map commands to enums for switching over in the system msg handler
    map_cmd_str_values["exit"] = ev_cmd1;
    map_cmd_str_values["new_game"] = ev_cmd2;
    map_cmd_str_values["end_game"] = ev_cmd3;
    map_cmd_str_values["end"] = ev_end;

    //  Set game flags
    sys_flag[IS_RUNNING] = true;
    sys_flag[GAME_STARTED] = false;
    sys_flag[GAME_MENU_OPENED] = true;

    //  Init done, set flag to true
    init_called = true;

    //  Blank the screen
    al_clear_to_color(WTE_COLOR_BLACK);
    al_flip_display();
}

//! Unload WTEngine
/*!
  Shut down the various Allegro objects
*/
inline void wte_main::wte_unload(void) {
    al_destroy_timer(main_timer);
    al_destroy_event_queue(main_queue);
    al_destroy_display(display);
    al_uninstall_system();

    init_called = false;
}

//! Call to generate a new game
/*!
  Call every time a new game is starting
*/
inline void wte_main::generate_new_game(void) {
    std::srand(std::time(nullptr));  //  Seed random

    //  Set global flags
    sys_flag[GAME_MENU_OPENED] = false;
    sys_flag[GAME_STARTED] = true;

    //  Clear world and load starting entities
    world.clear();
    load_game();

    //  Load a new message data file
    messages.new_data_file("data\\game.sdf");  //  Update later to load from settings

    //  Restart the timer at zero
    al_stop_timer(main_timer);
    al_set_timer_count(main_timer, 0);
    al_start_timer(main_timer);
}

//!  Call to stop the game in progress
/*
  Calls the user defined end game process, then shuts down the game
*/
inline void wte_main::unload_game(void) {
    al_stop_timer(main_timer);
    end_game();

    world.clear();
    sys_flag[GAME_STARTED] = false;
    sys_flag[GAME_MENU_OPENED] = true;
}

//! Main game logic
/*!
  The main game loop
*/
inline void wte_main::do_game(void) {
    if(init_called == false) throw std::runtime_error("WTEngine not initialized!");

    bool queue_not_empty = false;
    msg::message_container temp_msgs;

    generate_new_game(); //  test code

    while(sys_flag[IS_RUNNING]) {
        //  Pause / resume timer depending on if the game menu is opened
        if(sys_flag[GAME_MENU_OPENED] && al_get_timer_started(main_timer)) {
            menus.reset();
            al_stop_timer(main_timer);
        }
        if(!sys_flag[GAME_MENU_OPENED] && !al_get_timer_started(main_timer)) {
            menus.reset();
            al_resume_timer(main_timer);
        }

        //  Game menu is opened, run the menu manager
        if(sys_flag[GAME_MENU_OPENED]) menus.run(messages);

        /* *** GAME LOOP ************************************************************ */
        //  Capture event from queue
        queue_not_empty = al_get_next_event(main_queue, &event);
        //  Call our game logic update on timer events.  Timer is only running when the game is running.
        if(event.type == ALLEGRO_EVENT_TIMER && queue_not_empty) {
            //  Set the message queue's internal timer to the current time
            messages.set_time(al_get_timer_count(main_timer));
            //  Run all systems
            systems.run(world, messages, al_get_timer_count(main_timer));
            //  Process messages
            systems.dispatch(world, messages);
        }
        /* *** END GAME LOOP ******************************************************** */

        //  Render the screen
        game_screen.render(menus, world, al_get_timer_count(main_timer));

        //  Send audio messages to the audio queue
        temp_msgs = messages.get_messages("audio");
        if(!temp_msgs.empty()) audio_messages.insert(audio_messages.end(),
                                                     std::make_move_iterator(temp_msgs.begin()),
                                                     std::make_move_iterator(temp_msgs.end()));

        //  Get any system messages and pass to handler
        temp_msgs = messages.get_messages("system");
        if(!temp_msgs.empty()) handle_sys_msg(temp_msgs);

        //  Force quit if the game window is closed
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) sys_flag[IS_RUNNING] = false;
    }
}

//! Process system messages
/*!
  Switch over the system messages and process
  Remaining messages are passed to the custom handler
*/
inline void wte_main::handle_sys_msg(msg::message_container sys_msgs) {
    for(msg::message_iterator it = sys_msgs.begin(); it != sys_msgs.end();) {
        //  Switch over the system messages, deleting each as they are processed
        switch(map_cmd_str_values[it->get_cmd()]) {
            //  cmd:  exit - Shut down engine
            case ev_cmd1:
                if(sys_flag[GAME_STARTED] == true) unload_game();
                sys_flag[IS_RUNNING] = false;
                it = sys_msgs.erase(it);
                break;

            //  cmd:  new_game - start up a new game
            case ev_cmd2:
                if(sys_flag[GAME_STARTED] == true) unload_game();
                generate_new_game();
                it = sys_msgs.erase(it);
                break;

            //  cmd:  end_game - end current game
            case ev_cmd3:
                unload_game();
                it = sys_msgs.erase(it);
                break;

            //  cmd:  new_cmd - description
            //case ev_cmdX:
                //
                //it = sys_msgs.erase(it);
                //break;

            //  Command not defined, iterate to next
            default:
                it++;
        }
    }

    //  Pass remaining system messages to custom handler
    if(!sys_msgs.empty()) {
        for(msg::message_iterator it = sys_msgs.begin(); it != sys_msgs.end(); it++) {
            handle_custom_sys_msg(*it);
        }
    }
}

} //  end namespace wte

#endif
