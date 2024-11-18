/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_ENGINE_HPP)
#define SLV_ENGINE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#include "silvergun/config.hpp"
#include "silvergun/display.hpp"
#include "silvergun/input.hpp"

#include "silvergun/_debug/exceptions.hpp"
#include "silvergun/_debug/logger.hpp"
#include "silvergun/_globals/_defines.hpp"
#include "silvergun/_globals/commands.hpp"
#include "silvergun/_globals/engine_time.hpp"
#include "silvergun/_globals/scene.hpp"
#include "silvergun/_globals/slv_asset.hpp"
#include "silvergun/mgr/_managers.hpp"

namespace slv {

void start_game(void);
void stop_game(void);
#if defined(__EMSCRIPTEN__)
void em_looper(void);
#endif

/*!
 * \class engine
 * \brief The main engine class.
 * 
 * Sets up various system objects used by the engine.
 * Contains the main game loop and members for managing the game and engine.
 */
class engine final : public config, public input, public display {
  friend void start_game(void);
  friend void stop_game(void);
  #if defined(__EMSCRIPTEN__)
  friend void em_looper(void);
  #endif

  private:
    engine() = default;
    ~engine() = default;

    /*
     * Main engine loop (single pass)
     */
    static void main_loop(void) {
      input::check_events(current_scene->scope);  //  Check for input.

      //  Pause / resume timer check.  Also process the on_pause events.
      if (config::flags::engine_paused && al_get_timer_started(main_timer)) {
        al_stop_timer(main_timer);
        on_engine_pause();
      }
      if (!config::flags::engine_paused && !al_get_timer_started(main_timer)) {
        on_engine_unpause();
        al_resume_timer(main_timer);
      }

      ALLEGRO_EVENT event;
      if (al_get_next_event(main_event_queue, &event)) {
        switch (event.type) {
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
          stop();  //  Stop game
          break;
        //  Window has been resized.
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
          al_acknowledge_resize(_display);
          break;
        }
      }

      //  Run any custom scene code
      current_scene->loop();

      //  Render the screen.
      mgr::gfx::renderer::render();
      //  Process system messages
      cmds.process_messages(mgr::messages::get("system"));
      //  Send audio messages to the audio queue.
      mgr::audio::process_messages(mgr::messages::get("audio"));
      //  Delete unprocessed messages.
      mgr::messages::prune();
    };

    static void stop(void) {
      current_scene->unload();
      
      config::_flags::is_running = false;
    };

    //  Scenes
    inline static std::vector<std::shared_ptr<scene>> scenes;
    inline static std::shared_ptr<scene> current_scene = nullptr;

    //  Engine commands
    inline static commands cmds;

    //  Allegro objects used by the engine.
    inline static ALLEGRO_TIMER* main_timer = NULL;
    inline static ALLEGRO_EVENT_QUEUE* main_event_queue = NULL;

    //  Restrict to one instance of the engine running.
    inline static bool initialized = false;

  public:
    engine(const engine&) = delete;          //  Delete copy constructor.
    void operator=(engine const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Initialize the engine.
     * \param width Initial screen width.
     * \param height Initial screen height.
     */
    static void initialize(int width, int height) {
      std::cout << "Initializing Silvergun Game Engine...\n";
      if (initialized == true) throw engine_error(display::window_title + " already running!");
      initialized = true;

      std::cout << "Loading Allegro Game Library... ";
      //  Initialize Allegro.
      if (!al_init()) throw engine_error("Allegro failed to load!");
      std::cout << "OK!\n";

      //  Initialize additional Allegro components.
      std::cout << "Loading Allegro add-ons... ";
      if (!al_init_image_addon()) throw engine_error("Failed to load Allegro image addon!");
      if (!al_init_font_addon()) throw engine_error("Failed to load Allegro font addon!");
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

      //  Configure display.  Called from display class.
      std::cout << "Configuring display... ";
      create_display(width, height);
      std::cout << "OK!\n";

      //  Disable pesky screensavers.
      al_inhibit_screensaver(true);

      std::cout << "Creating main timer and event queue... ";
      //  Configure main timer.
      main_timer = al_create_timer(1.0f / build_options.ticks_per_sec);
      if (!main_timer) throw engine_error("Failed to create timer!");
      //  Configure main event queue.
      main_event_queue = al_create_event_queue();
      if (!main_event_queue) throw engine_error("Failed to create main event queue!");
      std::cout << "OK!\n";

      //  Register event sources.
      al_register_event_source(main_event_queue, al_get_display_event_source(_display));
      al_register_event_source(main_event_queue, al_get_timer_event_source(main_timer));

      //  Create the input event queue
      input::create_event_queue();

      //  Allegro extras
      al_init_primitives_addon();
      if (config::flags::audio_installed) al_init_acodec_addon();

      //  Load systems and prevent further systems from being loaded.
      std::cout << "Loading systems... ";
      load_systems();
      mgr::systems::finalized = true;
      if (mgr::systems::empty()) throw engine_error("No systems have been loaded!");
      std::cout << "OK!\n";

      std::cout << "Loading audio... ";
      mgr::audio::initialize();
      std::cout << "OK!\n";

      //  Generate Allegro's default font and load into asset mgr.
      mgr::assets::load<ALLEGRO_FONT>("slv_default_font", make_asset<ALLEGRO_FONT>());

      std::cout << "Initializing graphics... ";
      mgr::gfx::renderer::set_viewport_size(width, height);
      mgr::gfx::renderer::initialize();
      std::cout << "OK!\n";

      //  Engine (system) commands
      cmds.add("load-script", 1, [](const msg_args& args) {
        mgr::messages::load_script(args[0]);
      });
      cmds.add("load-scene", 1, [](const msg_args& args) {
        engine::load_scene(args[0]);
      });
      cmds.add("enable-input", 0, [](const msg_args& args) {
        config::flags::input_enabled = true;
      });
      cmds.add("disable-input", 0, [](const msg_args& args) {
        config::flags::input_enabled = false;
      });

      if constexpr (build_options.debug_mode) {
        std::cout << "DEBUG MODE --- LOGGING ENABLED\n";
        mgr::messages::message_log_start();
        logger::start();
      }

      config::flags::engine_paused = false;
      config::_flags::is_running = true;
      std::cout << "Engine started successfully!\n";

      al_stop_timer(main_timer);
      al_set_timer_count(main_timer, 0);
      engine_time::set(al_get_timer_count(main_timer));
      al_start_timer(main_timer);
    };

    /*!
     * \brief De-initialize the engine.
     */
    static void deinitialize(void) {
      std::cout << "\nDeinitializing Silvergun Game Engine...\n";

      config::_flags::is_running = false;

      mgr::world::clear();
      mgr::audio::deinitialize();
      mgr::gfx::renderer::deinitialize();
      mgr::assets::clear_al_objects();

      std::cout << "Cleaning up engine objects... ";
      al_destroy_timer(main_timer);
      al_destroy_event_queue(main_event_queue);
      input::destroy_event_queue();
      destroy_display();
      al_inhibit_screensaver(false);
      std::cout << "OK!\n";
      std::cout << "Stopping Allegro... ";
      al_shutdown_font_addon();
      if (config::flags::audio_installed) al_uninstall_audio();
      al_shutdown_primitives_addon();
      al_uninstall_system();
      std::cout << "OK!\n";

      if constexpr (build_options.debug_mode) {
        logger::stop();
        mgr::messages::message_log_stop();
      }

      initialized = false;
      std::cout << "Good bye!\n";
    };

    /*!
     * \brief Add a scene.
     */
    template <typename T, typename... Args>
    static void add_scene(Args... args) {
      scenes.push_back(std::make_shared<T>(args...));
    };

    /*!
     * \brief Load a scene.
     */
    static void load_scene(const std::string& name) {
      if (current_scene != nullptr) current_scene->unload();
      mgr::world::clear();
      mgr::messages::clear();

      const auto find_scene = [name](const std::shared_ptr<scene>& s) { return s->name == name; };
      if (auto it = std::find_if(scenes.begin(), scenes.end(), find_scene); it != scenes.end()) {
        current_scene = *it;
      } else {
        throw engine_error("Scene " + name + " does not exist!");
      }

      current_scene->load();
    };

    //!  Define this to load all systems to be used by the game.
    inline static std::function<void(void)> load_systems = [](){};
    //!  Optional:  On engine pause handler.
    inline static std::function<void(void)> on_engine_pause = [](){};
    //!  Optional:  On engine unpause handler.
    inline static std::function<void(void)> on_engine_unpause = [](){};
    //!  Optional:  Window out of focus handler.
    inline static std::function<void(void)> out_of_focus = [](){};
    //!  Optional:  Window back in focus handler.
    inline static std::function<void(void)> back_in_focus = [](){};
};

#if defined(__EMSCRIPTEN__)
inline void em_looper(void) {
  engine::main_loop();
  if (!config::flags::is_running) emscripten_cancel_main_loop();
}
#endif

/*!
 * \brief The main engine loop.
 */
inline void start_game(void) {
  //  MAIN ENGINE LOOP
  #if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(&em_looper, -1, true);
  #else
    while (config::flags::is_running) engine::main_loop();
  #endif
}

/*!
 * \brief Stop the running game.
 */
inline void stop_game(void) {
  engine::stop();
}

}  //  end namespace slv

#endif
