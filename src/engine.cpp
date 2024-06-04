/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/engine.hpp"

namespace wte {

commands engine::cmds;
std::vector<std::string> engine::file_locations;

/*
 *
 */
void engine::initialize(const int& argc, char** const& argv) {
  std::cout << "Starting WTEngine...\n";
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
  if constexpr (build_options.keyboard_enabled)
    config::_flags::keyboard_installed = al_install_keyboard();
  if constexpr (build_options.mouse_enabled)
    config::_flags::mouse_installed = al_install_mouse();
  if constexpr (build_options.joystick_enabled)    
    config::_flags::joystick_installed = al_install_joystick();
  if constexpr (build_options.touch_enabled)
    config::_flags::touch_installed = al_install_touch_input();

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
  main_timer = al_create_timer(1.0f / build_options.ticks_per_sec);
  if(!main_timer) throw engine_error("Failed to create timer!");
  //  Configure main event queue.
  main_event_queue = al_create_event_queue();
  if(!main_event_queue) throw engine_error("Failed to create main event queue!");
  std::cout << "OK!\n";

  //  Register event sources.
  al_register_event_source(main_event_queue, al_get_display_event_source(_display));
  al_register_event_source(main_event_queue, al_get_timer_event_source(main_timer));

  //  Create the input event queue
  input::create_event_queue();

  //  Allegro extras
  al_init_primitives_addon();
  if(config::flags::audio_installed) al_init_acodec_addon();

  //  Main engine commands.
  cmds.add("exit", 0, [](const msg_args& args) {
    if(config::flags::engine_started) process_end_game(true);
    config::_flags::is_running = false;
  });
  cmds.add("new-game", 1, [](const msg_args& args) {
    if(!config::flags::engine_started) {
      process_new_game(args[0]);
    }
  });
  cmds.add("end-game", 0, [](const msg_args& args) {
    if(config::flags::engine_started) {
      process_end_game(false);
    }
  });
  cmds.add("fps-counter", 1, [](const msg_args& args) {
    if(args[0] == "on") config::flags::draw_fps = true;
    if(args[0] == "off") config::flags::draw_fps = false;
  });
  cmds.add("load-script", 1, [](const msg_args& args) {
    if(config::flags::engine_started && args[0] != "") {
      if(!mgr::messages::load_script(args[0]))
        throw engine_exception("Error loading script:  " + args[0], "engine", 2);
    }
  });
  cmds.add("enable-input", 0, [](const msg_args& args) {
    config::flags::input_enabled = true;
  });
  cmds.add("disable-input", 0, [](const msg_args& args) {
    config::flags::input_enabled = false;
  });
  cmds.add("resize-display", 2, [](const msg_args& args) {
    display::resize_display(std::stoi(args[0]), std::stoi(args[1]));
  });
  cmds.add("display-mode", 1, [](const msg_args& args) {
    display::set_display_mode(std::stoul(args[0], NULL, 0));
  });
  cmds.add("scale-factor", 1, [](const msg_args& args) {
    display::set_scale_factor(std::stof(args[0]));
  });

  //  Asset commands.
  cmds.add("load-bitmap-asset", 2, [](const msg_args& args) {
    wte::mgr::assets::load<ALLEGRO_BITMAP>(args[0], wte::make_asset<ALLEGRO_BITMAP>(args[1]));
  });
  cmds.add("load-font-asset", 4, [](const msg_args& args) {
    wte::mgr::assets::load<ALLEGRO_FONT>(args[0], wte::make_asset<ALLEGRO_FONT>(args[1], std::stoi(args[2]), std::stoi(args[3])));
  });
  cmds.add("load-sample-asset", 2, [](const msg_args& args) {
    wte::mgr::assets::load<ALLEGRO_SAMPLE>(args[0], wte::make_asset<ALLEGRO_SAMPLE>(args[1]));
  });
  cmds.add("load-audio-asset", 2, [](const msg_args& args) {
    wte::mgr::assets::load<ALLEGRO_AUDIO_STREAM>(args[0], wte::make_asset<ALLEGRO_AUDIO_STREAM>(args[1]));
  });
  cmds.add("unload-bitmap-asset", 1, [](const msg_args& args) {
    wte::mgr::assets::unload<ALLEGRO_BITMAP>(args[0]);
  });
  cmds.add("unload-font-asset", 1, [](const msg_args& args) {
    wte::mgr::assets::unload<ALLEGRO_FONT>(args[0]);
  });
  cmds.add("unload-sample-asset", 1, [](const msg_args& args) {
    wte::mgr::assets::unload<ALLEGRO_SAMPLE>(args[0]);
  });
  cmds.add("unload-audio-asset", 1, [](const msg_args& args) {
    wte::mgr::assets::unload<ALLEGRO_AUDIO_STREAM>(args[0]);
  });

  if constexpr (build_options.debug_mode) {
    mgr::messages::message_log_start();
    logger::start();
  }
  std::cout << "Engine started successfully!\n\n";
}

}  //  end namespace wte
