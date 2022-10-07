/*!
 * wtengine | File:  engine.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/engine.hpp"

namespace wte {

commands engine::cmds;
ALLEGRO_TIMER* engine::main_timer = NULL;
ALLEGRO_EVENT_QUEUE* engine::main_event_queue = NULL;
std::vector<std::string> engine::file_locations;
bool engine::initialized = false;

/*
 *
 */
engine::engine(const int& argc, char** const& argv) {
    std::cout << "Starting wtengine...\n";
    if(initialized == true) throw engine_error(display::window_title + " already running!");
    initialized = true;

    std::cout << "Loading Allegro Game Library... ";
    //  Initialize Allegro.
    if(!al_init()) throw engine_error("Allegro failed to load!");
    std::cout << "OK!\n";

    //  Initialize additional Allegro components.
    std::cout << "Loading Allegro add-ons... ";
    if(!al_init_image_addon()) throw engine_error("Failed to load Allegro image addon!");
    if(!al_init_font_addon()) throw engine_error("Failed to load Allegro font addon!");
    std::cout << "OK!\n";
    config::_flags::audio_installed = al_install_audio();
    //  Input detection.
    config::_flags::keyboard_detected = al_install_keyboard();
    config::_flags::mouse_detected = al_install_mouse();
    config::_flags::joystick_detected = al_install_joystick();
    config::_flags::touch_detected = al_install_touch_input();

    //  Configure PhysFS.
    std::cout << "Loading PhysicsFS... ";
    if(!PHYSFS_init(argv[0])) throw engine_error("Failed to load PhysicsFS!");
    if(file_locations.empty()) throw engine_error("Need to configure locations for PhysFS!");
    for(auto& it: file_locations) PHYSFS_mount(it.c_str(), NULL, 1);
    al_set_physfs_file_interface();
    std::cout << "OK!\n";

    //  Configure display.  Called from wte_display class.
    std::cout << "Configuring display... ";
    create_display();
    std::cout << "OK!\n";

    //  Disable pesky screensavers.
    al_inhibit_screensaver(true);

    std::cout << "Creating main timer and event queue... ";
    //  Configure main timer.
    main_timer = al_create_timer(1.0f / ticks_per_sec);
    if(!main_timer) throw engine_error("Failed to create timer!");
    //  Configure main event queue.
    main_event_queue = al_create_event_queue();
    if(!main_event_queue) throw engine_error("Failed to create main event queue!");
    std::cout << "OK!\n";

    //  Register event sources.
    al_register_event_source(main_event_queue, al_get_display_event_source(_display));
    al_register_event_source(main_event_queue, al_get_timer_event_source(main_timer));

    input::create_event_queue();

    //  Main engine commands.
    cmds.add("exit", 0, [this](const msg_args& args) {
        if(config::flags::game_started) process_end_game(true);
        config::_flags::is_running = false;
    });
    cmds.add("new-game", 1, [this](const msg_args& args) {
        if(!config::flags::game_started) {
            process_new_game(args[0]);
        }
    });
    cmds.add("end-game", 0, [this](const msg_args& args) {
        if(config::flags::game_started) {
            process_end_game(false);
        }
    });
    cmds.add("fps-counter", 1, [this](const msg_args& args) {
        if(args[0] == "on") config::flags::draw_fps = true;
        if(args[0] == "off") config::flags::draw_fps = false;
    });
    cmds.add("load-script", 1, [this](const msg_args& args) {
        if(config::flags::game_started && args[0] != "") {
            if(!mgr::messages::load_script(args[0]))
                throw engine_exception("Error loading script:  " + args[0], "engine", 2);
        }
    });

    //if(build_options.debug_mode) logger::start();
    std::cout << "Engine started successfully!\n\n";
}

/*
 *
 */
engine::~engine() {
    std::cout << "Stopping wtengine... ";
    PHYSFS_deinit();

    al_destroy_timer(main_timer);
    al_destroy_event_queue(main_event_queue);
    input::destroy_event_queue();
    destroy_display();
    al_inhibit_screensaver(false);
    al_uninstall_system();

    //if(build_options.debug_mode) logger::stop();

    initialized = false;
    std::cout << "Done!\n\n";
}

/*
 *
 */
void engine::add_file_location(const std::string& flocation) {
    file_locations.push_back(flocation);
}

/*
 *
 */
void engine::wte_load(void) {
    //  Generate Allegro's default font and load into asset mgr.
    mgr::assets<al_font>::load<al_font>("wte_default_font", make_asset(al_font()));

    //  Initialize managers that require it.
    mgr::audio::initialize();
    mgr::gfx::renderer::initialize();
}

/*
 *
 */
void engine::wte_unload(void) {
    mgr::audio::de_init();
    mgr::gfx::renderer::de_init();
}

/*
 *
 */
void engine::process_new_game(const std::string& game_script) {
    std::cout << "Starting new game... ";
    try {
        std::srand(std::time(nullptr));  //  Seed random, using time.

        //  Make sure the menu isn't opened.
        config::_flags::menu_opened = false;

        //  Load a new message data file.
        if(!game_script.empty()) mgr::messages::load_file(game_script);

        //  Load systems and prevent further systems from being loaded.
        load_systems();
        mgr::systems::finalized = true;
        if(mgr::systems::empty()) throw engine_error("No systems have been loaded!");

        //  Stop audio manager from playing sounds.
        mgr::audio::music::a::stop();
        mgr::audio::music::b::stop();
        mgr::audio::ambiance::stop();
        mgr::audio::voice::stop();
        mgr::audio::sample::clear_instances();
        
        //  Clear world and load starting entities.
        mgr::world::clear();
        
        //  Call custom start game process
        new_game();

        //  Restart the timer at zero.
        al_stop_timer(main_timer);
        al_set_timer_count(main_timer, 0);
        engine_time::set(al_get_timer_count(main_timer));
        config::_flags::game_started = true;
        config::flags::input_enabled = true;
        al_start_timer(main_timer);
    } catch(const std::exception& e) { throw e; }
    std::cout << "DONE!\n";
}

/*
 *
 */
void engine::process_end_game(const bool& force) {
    std::cout << "Ending game... ";
    try {
        al_stop_timer(main_timer);
        config::_flags::game_started = false;
        config::flags::input_enabled = true;
        al_set_timer_count(main_timer, 0);
        engine_time::set(al_get_timer_count(main_timer));

        //  Stop audio manager from playing sounds.
        mgr::audio::music::a::stop();
        mgr::audio::music::b::stop();
        mgr::audio::ambiance::stop();
        mgr::audio::voice::stop();
        mgr::audio::sample::clear_instances();

        //  Call custom end game process.
        if(!force) end_game();

        //  Clear managers.
        mgr::world::clear();
        mgr::systems::clear();
        mgr::messages::clear();

        //  Open the menus.
        config::_flags::menu_opened = true;
    } catch(const std::exception& e) { throw e; }
    std::cout << "DONE!\n";
}

/*
 *
 */
void engine::do_game(void) {
    wte_load();

    //  Set default states.
    config::_flags::is_running = true;
    config::_flags::game_started = false;
    config::_flags::menu_opened = true;

    while(config::flags::is_running) {
        /* *** START ENGINE LOOP ******************************************** */
        input::check_events();  //  Check for input.

        if(!config::flags::game_started) {       //  Game not running.
            al_stop_timer(main_timer);           //  Make sure the timer isn't.
            config::_flags::menu_opened = true;  //  And force menus
        }
        //  Pause / resume timer depending on if the game menu is opened.
        //  Also process the on_menu events.
        if(config::flags::menu_opened && al_get_timer_started(main_timer)) {
            al_stop_timer(main_timer);
            on_menu_open();
        }
        if(!config::flags::menu_opened && !al_get_timer_started(main_timer)) {
            on_menu_close();
            al_resume_timer(main_timer);
        }

        ALLEGRO_EVENT event;
        if(al_get_next_event(main_event_queue, &event)) {
            switch(event.type) {
            //  Call our game logic update on timer events.
            //  Timer is only running when the game is running.
            case ALLEGRO_EVENT_TIMER:
                //  Set the engine_time object to the current time.
                engine_time::set(al_get_timer_count(main_timer));
                //  Run all systems.
                mgr::systems::run();
                //  Process messages.
                mgr::messages::dispatch();
                //  Get any spawner messages and pass to handler.
                mgr::spawner::process_messages(mgr::messages::get("spawner"));
                break;
            //  Check if display looses focus.
            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
                out_of_focus();
                break;
            //  Check if display returns to focus.
            case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                back_in_focus();
                break;
            //  Force quit if the game window is closed.
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                if(config::flags::game_started) process_end_game(true);
                config::_flags::is_running = false;
                break;
            //  Window has been resized.
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                //
                break;
            }
        }

        //  Render the screen.
        mgr::gfx::renderer::render();
        //  Get any system messages and pass to handler.
        cmds.process_messages(mgr::messages::get("system"));
        //  Send audio messages to the audio queue.
        mgr::audio::process_messages(mgr::messages::get("audio"));
        //  Delete unprocessed messages.
        mgr::messages::prune();
        /* *** END ENGINE LOOP ********************************************** */
    }

    wte_unload();
}

}  //  end namespace wte
