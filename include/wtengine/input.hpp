/*!
 * wtengine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_INPUT_HPP
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
#include <stdexcept>

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

        /*!
         * \struct states
         * \brief Input States.
         */
        struct states {
            static const ALLEGRO_KEYBOARD_STATE& keyboard;    //<!  Keyboard input state.
            static const ALLEGRO_MOUSE_STATE& mouse;          //<!  Mouse input state.
            static const ALLEGRO_TOUCH_INPUT_STATE& touches;  //<!  Array of Allegro touch states.
            static const ALLEGRO_TOUCH_STATE& first_touch;    //<!  First touch input state.
            static const ALLEGRO_TOUCH_STATE& last_touch;     //<!  Last touch input state.
        };

        /*!
         * \struct lastkeypress
         * \brief Record last key press.
         */
        struct lastkeypress {
            static const int64_t& timer;  //!<  Time the key was pressed.
            static const int& key;        //!<  Keycode of key pressed.
        };

        /*!
         * \struct lastbuttonpress
         * \brief Record last button press.
         */
        struct lastbuttonpress {
            static const int64_t& timer;  //!<  Time the button was pressed.
            static const int& button;     //!<  Button code of button pressed.
        };

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

        static void create_event_queue(void);                 //  Create the input queue.
        static void destroy_event_queue(void);                //  Destroy the input queue.
        static void record_event(const ALLEGRO_EVENT&);       //  Record input events.
        static const bool save_recorder(void);                //  Save input events.
        static const bool check_events(void);                 //  Check the input queue for events.
        static void capture_states(const ALLEGRO_EVENT&);     //  Capture input states.
        static void run_game_handler(const ALLEGRO_EVENT&);  //  Process in-game input events.

        struct _states {
            inline static ALLEGRO_KEYBOARD_STATE keyboard;
            inline static ALLEGRO_MOUSE_STATE mouse;
            //inline static ALLEGRO_JOYSTICK_STATE joystick;
            inline static ALLEGRO_TOUCH_INPUT_STATE touches;
            inline static ALLEGRO_TOUCH_STATE first_touch;
            inline static ALLEGRO_TOUCH_STATE last_touch;
        };

        struct _lastkeypress {
            inline static int64_t timer = 0;
            inline static int key = 0;
        };

        struct _lastbuttonpress {
            inline static int64_t timer = 0;
            inline static int button = 0;
        };

        static int64_t last_tick;
        static std::vector<
            std::pair<const int64_t, const std::vector<ALLEGRO_EVENT>>
        > input_recorder;
        static std::vector<ALLEGRO_EVENT> event_recorder;

        static ALLEGRO_EVENT_QUEUE* input_event_queue;  //  Input event queue.
        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
