/*!
 * wtengine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_INPUT_HPP)
#define WTE_INPUT_HPP

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <functional>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <cmath>

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
        virtual ~input() = default;             //!<  Default virtual destructor.
        input(const input&) = delete;           //!<  Delete copy constructor.
        void operator=(input const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Toggle input recording.
         */
        static void toggle_recording(void);

    protected:
        input();  //!<  Constructor

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

        static void create_event_queue(void);                //  Create the input queue.
        static void destroy_event_queue(void);               //  Destroy the input queue.
        static void record_event(const int64_t&, const ALLEGRO_EVENT&);      //  Record input events.
        static bool check_events(void);                      //  Check the input queue for events.
        static void run_game_handler(const ALLEGRO_EVENT&);  //  Process in-game input events.

        static ALLEGRO_EVENT_QUEUE* input_event_queue;  //  Input event queue.
        static std::ofstream input_event_file;
        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
