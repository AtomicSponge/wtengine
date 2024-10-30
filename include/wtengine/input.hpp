/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_INPUT_HPP)
#define WTE_INPUT_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <fstream>
#include <sstream>
#include <ctime>

#include <allegro5/allegro.h>

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/handlers.hpp"
#include "wtengine/config.hpp"

namespace wte {

/*!
 * \class input
 * \brief Manage the input queue and process events.
 */
class input {
  friend class engine;

  public:
    virtual ~input() = default;             //  Default virtual destructor.
    input(const input&) = delete;           //  Delete copy constructor.
    void operator=(input const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Start input recording.
     */
    static void start_recording(void) {
      input_event_file.open("input_events", std::ios::binary | std::ofstream::app);
      config::_flags::record_input = true;
    };

    /*!
     * \brief Stop input recording.
     */
    static void stop_recording(void) {
      if(input_event_file.is_open()) input_event_file.close();
      config::_flags::record_input = false;
    };

  protected:
    //  Constructor
    input() {
      if(initialized == true) throw engine_error("Input instance already running!");
      initialized = true;
    };

  private:
    template <handler_scopes S>
    constexpr inline static void run_handles(const ALLEGRO_EVENT& event) {
      //  Keyboard events
      if constexpr (build_options.keyboard_enabled) {
        if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
          std::get<handler::key>(handlers<S, EVENT_KEY_DOWN, handler::key>::_handle)(
            event.keyboard.keycode, event.keyboard.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_KEY_UP) {
          std::get<handler::key>(handlers<S, EVENT_KEY_UP, handler::key>::_handle)(
            event.keyboard.keycode, event.keyboard.display);
          return;
        }
      }

      //  Mouse events
      if constexpr (build_options.mouse_enabled) {
        if(event.type == ALLEGRO_EVENT_MOUSE_AXES) {
          std::get<handler::mouse_axis>(handlers<S, EVENT_MOUSE_AXIS, handler::mouse_axis>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
            event.mouse.pressure, event.mouse.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
          std::get<handler::mouse_button>(handlers<S, EVENT_MOUSE_BUTTON_DOWN, handler::mouse_button>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.button, event.mouse.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
          std::get<handler::mouse_button>(handlers<S, EVENT_MOUSE_BUTTON_UP, handler::mouse_button>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.button, event.mouse.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_MOUSE_WARPED) {
          std::get<handler::mouse_axis>(handlers<S, EVENT_MOUSE_WARPED, handler::mouse_axis>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
            event.mouse.pressure, event.mouse.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
          std::get<handler::mouse_axis>(handlers<S, EVENT_MOUSE_ENTER_DISPLAY, handler::mouse_axis>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
            event.mouse.pressure, event.mouse.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
          std::get<handler::mouse_axis>(handlers<S, EVENT_MOUSE_LEAVE_DISPLAY, handler::mouse_axis>::_handle)(
            event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
            event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
            event.mouse.pressure, event.mouse.display);
          return;
        }
      }

      //  Joystick events
      if constexpr (build_options.joystick_enabled) {
        if(event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
          std::get<handler::joystick_axis>(handlers<S, EVENT_JOYSTICK_AXIS, handler::joystick_axis>::_handle)(
            event.joystick.stick, event.joystick.axis, event.joystick.pos, event.joystick.id);
          return;
        }
        if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
          std::get<handler::joystick_button>(handlers<S, EVENT_JOYSTICK_BUTTON_DOWN, handler::joystick_button>::_handle)(
            event.joystick.button, event.joystick.id);
          return;
        }
        if(event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
          std::get<handler::joystick_button>(handlers<S, EVENT_JOYSTICK_BUTTON_UP, handler::joystick_button>::_handle)(
            event.joystick.button, event.joystick.id);
          return;
        }
      }

      //  Touch events
      if constexpr (build_options.touch_enabled) {
        if(event.type == ALLEGRO_EVENT_TOUCH_BEGIN) {
          std::get<handler::touch>(handlers<S, EVENT_TOUCH_BEGIN, handler::touch>::_handle)(
            event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_TOUCH_END) {
          std::get<handler::touch>(handlers<S, EVENT_TOUCH_END, handler::touch>::_handle)(
            event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_TOUCH_MOVE) {
          std::get<handler::touch>(handlers<S, EVENT_TOUCH_MOVE, handler::touch>::_handle)(
            event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
          return;
        }
        if(event.type == ALLEGRO_EVENT_TOUCH_CANCEL) {
          std::get<handler::touch>(handlers<S, EVENT_TOUCH_CANCEL, handler::touch>::_handle)(
            event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
          return;
        }
      }
    };


    //  Create the input queue.
    static void create_event_queue(void) {
      input_event_queue = al_create_event_queue();
      if(!input_event_queue) throw engine_error("Failed to create input event queue!");

      if(build_options.keyboard_enabled && config::flags::keyboard_installed)
        al_register_event_source(input_event_queue, al_get_keyboard_event_source());
      if(build_options.mouse_enabled && config::flags::mouse_installed)
        al_register_event_source(input_event_queue, al_get_mouse_event_source());
      if(build_options.joystick_enabled && config::flags::joystick_installed)
        al_register_event_source(input_event_queue, al_get_joystick_event_source());
      if(build_options.touch_enabled && config::flags::touch_installed)
        al_register_event_source(input_event_queue, al_get_touch_input_event_source());
    };

    //  Destroy the input queue.
    static void destroy_event_queue(void) {
      al_destroy_event_queue(input_event_queue);
    };

    //  Record input events
    static void record_event(const int64_t& time, const ALLEGRO_EVENT& event) { 
      input_event_file.write(reinterpret_cast<const char*>(time), sizeof(int64_t));
      input_event_file.write(reinterpret_cast<const char*>(sizeof(event)), sizeof(std::size_t));
      input_event_file.write(reinterpret_cast<const char*>(&event), sizeof(event));
    };

    //  Check the input queue for events.
    static void check_events(void) {
      ALLEGRO_EVENT event;
      while(al_get_next_event(input_event_queue, &event)) {
        if constexpr (build_options.debug_mode) {
          if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == WTE_KEY_TOGGLE_HITBOX)
            (config::flags::show_hitboxes ?
                config::_flags::show_hitboxes = false :
                config::_flags::show_hitboxes = true);
        }
        //  Record input if enabled.
        if(config::flags::record_input) record_event(engine_time::check(), event);
        //  Run the handles
        run_handles<GLOBAL_HANDLES>(event);        //  Run global handles
        if(config::flags::engine_started) {
          if(config::flags::input_enabled)
            run_handles<GAME_HANDLES>(event);  //  Run game handles
        } else {
          run_handles<NONGAME_HANDLES>(event);   //  Run non-game handles
        }
      }
    };

    //  Process in-game input events.
    static void run_game_handler(const ALLEGRO_EVENT&);

    inline static ALLEGRO_EVENT_QUEUE* input_event_queue;  //  Input event queue.
    inline static std::ofstream input_event_file;          //  Event record file.
    inline static bool initialized = false;                //  Restrict to one instance.
};

}  //  end namespace wte

#endif
