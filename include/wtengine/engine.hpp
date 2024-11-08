/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_ENGINE_HPP)
#define WTE_ENGINE_HPP

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

#include "wtengine/config.hpp"
#include "wtengine/display.hpp"
#include "wtengine/input.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_debug/logger.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/commands.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/scene.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/_managers.hpp"

namespace wte {

void do_game(void);
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
  friend void do_game(void);
  #if defined(__EMSCRIPTEN__)
  friend void em_looper(void);
  #endif

  private:
    engine() = default;
    ~engine() = default;

    /*
     * Load the engine's managers.
     * Called before the main loop starts.
     */
    static void wte_load(void) {
      //  Generate Allegro's default font and load into asset mgr.
      mgr::assets::load<ALLEGRO_FONT>("wte_default_font", make_asset<ALLEGRO_FONT>());

      //  Initialize managers that require it.
      mgr::audio::initialize();
      mgr::gfx::renderer::initialize();

      //  Set default states.
      config::_flags::is_running = true;
      config::_flags::engine_started = false;
      config::flags::engine_paused = false;
    };

    /*
     * Unload the engine's managers.
     * Called after the main loop ends running.
     */
    static void wte_unload(void) {
      mgr::audio::de_init();
      mgr::gfx::renderer::de_init();
      mgr::assets::clear_al_objects();
    };

    /*
     * Main engine loop (single pass)
     */
    static void main_loop(void) {
      input::check_events();  //  Check for input.

      //  Game not running, make sure the timer isn't.
      if (!config::flags::engine_started) al_stop_timer(main_timer);
      else {
        //  Pause / resume timer check.  Also process the on_pause events.
        if (config::flags::engine_paused && al_get_timer_started(main_timer)) {
          al_stop_timer(main_timer);
          on_engine_pause();
        }
        if (!config::flags::engine_paused && !al_get_timer_started(main_timer)) {
          on_engine_unpause();
          al_resume_timer(main_timer);
        }
      }

      ALLEGRO_EVENT event;
      if (al_get_next_event(main_event_queue, &event)) {
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
          if (config::flags::engine_started) process_end_game(true);
          config::_flags::is_running = false;
          break;
        //  Window has been resized.
        case ALLEGRO_EVENT_DISPLAY_RESIZE:
          al_acknowledge_resize(_display);
          break;
        }
      }

      //  Render the screen.
      mgr::gfx::renderer::render();
      //  Send audio messages to the audio queue.
      mgr::audio::process_messages(mgr::messages::get("audio"));
      //  Delete unprocessed messages.
      mgr::messages::prune();
    };

    /*
     * Call to end the game.
     * Clears out the entities and systems and runs user defined end process.
     * If passed true, skips the custom game cleanup.
     */
    static void process_end_game(const bool& force) {
      std::cout << "Ending game... ";
      al_stop_timer(main_timer);
      config::_flags::engine_started = false;
      al_set_timer_count(main_timer, 0);
      engine_time::set(al_get_timer_count(main_timer));

      //  Stop audio manager from playing sounds.
      mgr::audio::music::a::stop();
      mgr::audio::music::b::stop();
      mgr::audio::ambiance::stop();
      mgr::audio::voice::stop();
      mgr::audio::sample::clear_instances();

      //  Call custom end game process.
      if (!force) end_game();

      //  Clear managers.
      mgr::world::clear();
      mgr::systems::clear();
      mgr::messages::clear();
      std::cout << "DONE!\n";
    };

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
     * \param w Initial screen width.
     * \param h Initial screen height.
     */
    static void initialize(int w, int h) {
      std::cout << "Starting WTEngine...\n";
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

      //  Configure display.  Called from wte_display class.
      std::cout << "Configuring display... ";
      create_display(w, h);
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

      if constexpr (build_options.debug_mode) {
        mgr::messages::message_log_start();
        logger::start();
      }
      std::cout << "Engine started successfully!\n\n";
    };

    /*!
     * \brief De-initialize the engine.
     */
    static void de_init(void) {
      std::cout << "\nStopping WTEngine...\n";

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
      std::cout << "\nGood bye!\n\n";
    };

    /*
     * Call to start a new game.
     * Loads a game data file and user defined systems and starting entities.
     * Gets passed game data file to load.
     */
    static void load_scene(void) {
      std::cout << "Starting new game... ";

      //  Load systems and prevent further systems from being loaded.
      load_systems();
      mgr::systems::finalized = true;
      if (mgr::systems::empty()) throw engine_error("No systems have been loaded!");

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
      config::_flags::engine_started = true;
      config::flags::engine_paused = false;
      al_start_timer(main_timer);
      std::cout << "READY!\n";
    };

    //!  Define this to load all systems to be used by the game.
    inline static std::function<void(void)> load_systems = [](){};
    //!  Define what gets loaded when a game starts.
    inline static std::function<void(void)> new_game = [](){};
    //!  Define what happens at the end of a game.
    inline static std::function<void(void)> end_game = [](){};
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
inline void do_game(void) {
  //  Load engine.
  engine::wte_load();

  //  MAIN ENGINE LOOP
  #if defined(__EMSCRIPTEN__)
    emscripten_set_main_loop(&em_looper, -1, true);
  #else
    while (config::flags::is_running) engine::main_loop();
  #endif

  // Unload engine.
  engine::wte_unload();
}

}  //  end namespace wte

#endif
