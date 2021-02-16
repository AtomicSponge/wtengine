/*!
 * WTEngine | File:  wte_main.cpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_physfs.h>
#include <physfs.h>

#include "wtengine/wte_main.hpp"

namespace wte
{

wte_main::~wte_main() {
    PHYSFS_deinit();

    al_destroy_timer(main_timer);
    al_destroy_event_queue(main_event_queue);
    destroy_input_event_queue();
    destroy_display();
    al_inhibit_screensaver(false);
    al_uninstall_system();

    initialized = false;
};

static void wte_main::add_file_location(const std::string& flocation) {
    file_locations.push_back(flocation);
};

wte_main::wte_main(const int argc, char **argv, const std::string& title) :
wte_display(title) {
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
    al_register_event_source(main_event_queue, al_get_display_event_source(display));
    al_register_event_source(main_event_queue, al_get_timer_event_source(main_timer));

    create_input_event_queue();

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
    map_cmd_str_values["reconf_display"] = CMD_STR_RECONF_DISPLAY;
    map_cmd_str_values["fps_counter"] = CMD_STR_FPS_COUNTER;
    map_cmd_str_values["load_script"] = CMD_STR_LOAD_SCRIPT;

    //  Set default colors for alerts.
    alert::set_font_color(WTE_COLOR_WHITE);
    alert::set_bg_color(WTE_COLOR_RED);

    //  Check to see if FPS drawing should be enabled.
    if(engine_cfg::is_reg("draw_fps")) {
        if(engine_cfg::get<bool>("draw_fps")) engine_flags::set(DRAW_FPS);
        else engine_flags::unset(DRAW_FPS);
    }
};

void wte_main::wte_load(void) {
    //  Initialize managers that require it.
    screen.initialize();
    menus.initialize();
    audio_th.initialize();

    //  Start the audio thread.
    audio_th.start();

    //  Load user configured menus.
    load_menus();
};

void wte_main::wte_unload(void) {
    audio_th.stop();

    screen.de_init();
    menus.de_init();
    audio_th.de_init();
};

void wte_main::process_new_game(const std::string& game_data) {
    std::srand(std::time(nullptr));  //  Seed random, using time.

    //  Make sure the menu isn't opened.
    engine_flags::unset(GAME_MENU_OPENED);

    //  Load a new message data file.
    if(!game_data.empty()) messages.load_file(game_data);

    //  Load systems and prevent further systems from being loaded.
    load_systems();
    systems.finalize();
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    //  Clear world and load starting entities.
    world.clear();
    new_game();

    //  Stop audio manager from playing sounds.
    //  Messages run in reverse order.
    //  Messages from new_game() will run AFTER these.
    messages.add_message(message("audio", "stop_music", ""));
    messages.add_message(message("audio", "stop_ambiance", ""));
    messages.add_message(message("audio", "stop_voice", ""));
    messages.add_message(message("audio", "unload_sample", "all"));
    messages.add_message(message("audio", "clear_instances", ""));

    //  Restart the timer at zero.
    al_stop_timer(main_timer);
    al_set_timer_count(main_timer, 0);
    engine_flags::set(GAME_STARTED);
    mgr::engine_time::set_time(al_get_timer_count(main_timer));
    al_start_timer(main_timer);
}

void wte_main::process_end_game(void) {
    al_stop_timer(main_timer);
    engine_flags::unset(GAME_STARTED);
    al_set_timer_count(main_timer, 0);
    mgr::engine_time::set_time(al_get_timer_count(main_timer));

    //  Stop audio manager from playing sounds.
    //  Messages run in reverse order.
    //  Messages from end_game() will run BEFORE these.
    messages.add_message(message("audio", "stop_music", ""));
    messages.add_message(message("audio", "stop_ambiance", ""));
    messages.add_message(message("audio", "stop_voice", ""));
    messages.add_message(message("audio", "unload_sample", "all"));
    messages.add_message(message("audio", "clear_instances", ""));

    //  Call end game process.
    end_game();

    //  Clear world and systems.
    world.clear();
    systems.clear();

    //  Open the menus.
    engine_flags::set(GAME_MENU_OPENED);
}

void wte_main::do_game(void) {
    wte_load();

    //  Set default states.
    engine_flags::set(IS_RUNNING);
    engine_flags::unset(GAME_STARTED);
    engine_flags::set(GAME_MENU_OPENED);

    input_flags::unset_all();

    while(engine_flags::is_set(IS_RUNNING)) {
        if(!engine_flags::is_set(GAME_STARTED)) {  //  Game not running.
            al_stop_timer(main_timer);             //  Make sure the timer isn't.
            engine_flags::set(GAME_MENU_OPENED);   //  And force the menu manager.
        }

        //  Pause / resume timer depending on if the game menu is opened.
        //  Also process the on_menu events.
        if(engine_flags::is_set(GAME_MENU_OPENED) && al_get_timer_started(main_timer)) {
            al_stop_timer(main_timer);
            audio_th.get_volume();  //  Make sure engine cfg matches audio manager.
            on_menu_open();
        }
        if(!engine_flags::is_set(GAME_MENU_OPENED) && !al_get_timer_started(main_timer)) {
            on_menu_close();
            al_resume_timer(main_timer);
        }

        //  Check for input.
        check_input_events();

        //  Game menu is opened, run the menu manager.
        if(engine_flags::is_set(GAME_MENU_OPENED)) menus.run(messages);

        message_container temp_msgs;

        /* *** GAME LOOP ************************************************************ */
        ALLEGRO_EVENT event;
        const bool queue_not_empty = al_get_next_event(main_event_queue, &event);
        //  Call our game logic update on timer events.  Timer is only running when the game is running.
        if(queue_not_empty && event.type == ALLEGRO_EVENT_TIMER) {
            const int64_t the_time = al_get_timer_count(main_timer);
            //  Set the engine_time object to the current time.
            mgr::engine_time::set_time(the_time);

            //  Run all systems.
            systems.run(world, messages, the_time);
            //  Process messages.
            systems.dispatch(world, messages, the_time);

            //  Get any spawner messages and pass to handler.
            temp_msgs = messages.get_messages("spawner");
            if(!temp_msgs.empty()) spawner.process(temp_msgs, world);
        }
        /* *** END GAME LOOP ******************************************************** */

        //  Render the screen.
        screen.render(menus, world);

        //  Get any system messages and pass to handler.
        temp_msgs = messages.get_messages("system");
        if(!temp_msgs.empty()) handle_sys_msg(temp_msgs);

        //  Send audio messages to the audio queue.
        temp_msgs = messages.get_messages("audio");
        if(!temp_msgs.empty()) audio_th.transfer_messages(temp_msgs);

        //  Ignore message pruning if WTE_NO_PRUNE build flag is defined.
        #if WTE_PRUNE_ENABLED
        //  Delete timed messages that were not processed.
        messages.prune();
        #endif

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
            if(engine_flags::is_set(GAME_STARTED)) process_end_game();
            engine_flags::unset(IS_RUNNING);
        }
    }  //  End game loop.

    wte_unload();
}

void wte_main::handle_sys_msg(message_container& sys_msgs) {
    const bool timer_running = al_get_timer_started(main_timer);

    for(auto it = sys_msgs.begin(); it != sys_msgs.end();) {
        //  Switch over the system messages, deleting each as they are processed.
        switch(map_cmd_str_values[it->get_cmd()]) {
            //  CMD:  exit - Shut down engine.
            case CMD_STR_EXIT:
                if(engine_flags::is_set(GAME_STARTED)) process_end_game();
                engine_flags::unset(IS_RUNNING);
                it = sys_msgs.erase(it);
                break;

            //  CMD:  alert - Display an alert.
            case CMD_STR_ALERT:
                alert::set_alert(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  new_game - Start up a new game.
            case CMD_STR_NEW_GAME:
                //  If the game is running, ignore.
                if(!engine_flags::is_set(GAME_STARTED)) {
                    menus.reset();
                    process_new_game(it->get_arg(0));
                }
                it = sys_msgs.erase(it);
                break;

            //  CMD:  end_game - End current game.
            case CMD_STR_END_GAME:
                //  If the game not is running, ignore.
                if(engine_flags::is_set(GAME_STARTED)) {
                    process_end_game();
                    menus.reset();
                }
                it = sys_msgs.erase(it);
                break;

            //  CMD:  open_menu argstring - Open a menu, passing a string as an argument.
            //  If the menu doesn't exist, the default will be opened.
            case CMD_STR_OPEN_MENU:
                menus.open_menu(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  close_menu argstring - Close the opened menu.
            //  If argstring = "all", close all opened menus.
            case CMD_STR_CLOSE_MENU:
                if(it->get_arg(0) == "all") menus.reset();
                else menus.close_menu();
                it = sys_msgs.erase(it);
                break;

            //  CMD:  enable_system - Turn a system on for processing.
            case CMD_STR_ENABLE_SYSTEM:
                if(engine_flags::is_set(GAME_STARTED))
                    systems.enable_system(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  disable_system - Turn a system off so it's run member is skipped.
            //  Message dispatching is still processed.
            case CMD_STR_DISABLE_SYSTEM:
                if(engine_flags::is_set(GAME_STARTED))
                    systems.disable_system(it->get_arg(0));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  set_engcfg - Set engine cfg variables.
            //  Arguments:  var=val
            case CMD_STR_SET_ENGCFG:
                for(std::size_t i = 0; i < it->num_args(); i++)
                    engine_cfg::set(it->get_arg(i));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  set_gamecfg - Set game cfg variables.
            //  Arguments:  var=val
            case CMD_STR_SET_GAMECFG:
                for(std::size_t i = 0; i < it->num_args(); i++)
                    game_cfg::set(it->get_arg(i));
                it = sys_msgs.erase(it);
                break;

            //  CMD:  reconf_display - Reconfigure the display.
            case CMD_STR_RECONF_DISPLAY:
                //  Make sure the timer isn't running and unregister the display.
                if(timer_running) al_stop_timer(main_timer);
                al_pause_event_queue(main_event_queue, true);
                al_unregister_event_source(main_event_queue, al_get_display_event_source(display));
                //  Reload the display.
                reconf_display();
                //  Reload any temp bitmaps.
                screen.reload_arena_bitmap();
                menus.reload_menu_bitmap();
                if(engine_flags::is_set(GAME_STARTED)) systems.reload_temp_bitmaps(world);
                //  Register display event source and resume timer if it was running.
                al_register_event_source(main_event_queue, al_get_display_event_source(display));
                al_pause_event_queue(main_event_queue, false);
                if(timer_running) al_resume_timer(main_timer);
                it = sys_msgs.erase(it);
                break;

            //  CMD:  fps_counter - Enable/disable on-screen fps counter.
            case CMD_STR_FPS_COUNTER:
                if(it->get_arg(0) == "on") engine_flags::set(DRAW_FPS);
                if(it->get_arg(0) == "off") engine_flags::unset(DRAW_FPS);
                if(engine_cfg::is_reg("draw_fps")) {
                    if(it->get_arg(0) == "on") engine_cfg::set("draw_fps=1");
                    if(it->get_arg(0) == "off") engine_cfg::set("draw_fps=0");
                } else {
                    if(it->get_arg(0) == "on") engine_cfg::reg("draw_fps=1");
                    if(it->get_arg(0) == "off") engine_cfg::reg("draw_fps=0");
                }
                it = sys_msgs.erase(it);
                break;

            //  CMD:  load_script - Load a script into the message queue.
            case CMD_STR_LOAD_SCRIPT:
                if(engine_flags::is_set(GAME_STARTED) && it->get_arg(0) != "") {
                    if(!messages.load_script(it->get_arg(0)))
                        alert::set_alert("Error loading script:  " + it->get_arg(0));
                }
                it = sys_msgs.erase(it);
                break;

            //  CMD:  new_cmd - description
            //case CMD_STR_X:
                //
                //it = sys_msgs.erase(it);
                //break;

            //  Command not defined, iterate to next.
            default:
                it++;
        }
    }

    //  Pass remaining system messages to the custom handler.
    if(!sys_msgs.empty()) for(auto & it : sys_msgs) handle_custom_sys_msg(it);
}

} //  end namespace wte
