/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/handlers.hpp"
#include "wtengine/_globals/wte_exception.hpp"
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
        inline static constexpr void run_handles(const ALLEGRO_EVENT& event) {
            /*switch(event.type) {
            //  Keyboard events
            case ALLEGRO_EVENT_KEY_DOWN:
                if constexpr (handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_KEY_DOWN>::is_set())
                std::get<handler::key>(handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_KEY_DOWN>::_handle)(
                    event.keyboard.keycode, event.keyboard.display);
                break;
            case ALLEGRO_EVENT_KEY_UP:
                if constexpr (handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_KEY_UP>::is_set())
                std::get<handler::key>(handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_KEY_UP>::_handle)(
                    event.keyboard.keycode, event.keyboard.display);
                break;

            //  Mouse events
            case ALLEGRO_EVENT_MOUSE_AXES:
                if constexpr (handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_AXES>::is_set())
                std::get<handler::mouse_axis>(handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_AXES>::_handle)(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                    event.mouse.pressure, event.mouse.display);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if constexpr (handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_BUTTON_DOWN>::is_set())
                std::get<handler::mouse_button>(handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_BUTTON_DOWN>::_handle)(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.button, event.mouse.display);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if constexpr (handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_BUTTON_UP>::is_set())
                std::get<handler::mouse_button>(handlers<WTE_GLOBAL_HANDLES, WTE_EVENT_MOUSE_BUTTON_UP>::_handle)(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.button, event.mouse.display);
                break;
            case ALLEGRO_EVENT_MOUSE_WARPED:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_MOUSE_WARPED>())
                std::get<handler::mouse_axis>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_MOUSE_WARPED])(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                    event.mouse.pressure, event.mouse.display);
                break;
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_MOUSE_ENTER_DISPLAY>())
                std::get<handler::mouse_axis>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_MOUSE_ENTER_DISPLAY])(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                    event.mouse.pressure, event.mouse.display);
                break;
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_MOUSE_LEAVE_DISPLAY>())
                std::get<handler::mouse_axis>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_MOUSE_LEAVE_DISPLAY])(
                    event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                    event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                    event.mouse.pressure, event.mouse.display);
                break;

            //  Joystick events
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_JOYSTICK_AXIS>())
                std::get<handler::joystick_axis>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_AXIS])(
                    event.joystick.stick, event.joystick.axis, event.joystick.pos, event.joystick.id);
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_DOWN>())
                std::get<handler::joystick_button>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_DOWN])(
                    event.joystick.button, event.joystick.id);
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_UP>())
                std::get<handler::joystick_button>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_UP])(
                    event.joystick.button, event.joystick.id);
                break;

            //  Touch events
            case ALLEGRO_EVENT_TOUCH_BEGIN:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_TOUCH_BEGIN>())
                std::get<handler::touch>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_TOUCH_BEGIN])(
                    event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                break;
            case ALLEGRO_EVENT_TOUCH_END:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_TOUCH_END>())
                std::get<handler::touch>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_TOUCH_END])(
                    event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                break;
            case ALLEGRO_EVENT_TOUCH_MOVE:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_TOUCH_MOVE>())
                std::get<handler::touch>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_TOUCH_MOVE])(
                    event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                break;
            case ALLEGRO_EVENT_TOUCH_CANCEL:
                if constexpr (handlers<WTE_GLOBAL_HANDLES>::is_set<WTE_EVENT_TOUCH_CANCEL>())
                std::get<handler::touch>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_TOUCH_CANCEL])(
                    event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                break;
            }
            if(config::flags::game_started)
                switch(event.type) {
                //  Keyboard events
                case ALLEGRO_EVENT_KEY_DOWN:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_KEY_DOWN>())
                    std::get<handler::key>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_KEY_DOWN])(
                        event.keyboard.keycode, event.keyboard.display);
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_KEY_UP>())
                    std::get<handler::key>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_KEY_UP])(
                        event.keyboard.keycode, event.keyboard.display);
                    break;

                //  Mouse events
                case ALLEGRO_EVENT_MOUSE_AXES:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_AXES>())
                    std::get<handler::mouse_axis>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_AXES])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_BUTTON_DOWN>())
                    std::get<handler::mouse_button>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_BUTTON_DOWN])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.button, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_BUTTON_UP>())
                    std::get<handler::mouse_button>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_BUTTON_UP])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.button, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_WARPED:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_WARPED>())
                    std::get<handler::mouse_axis>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_WARPED])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_ENTER_DISPLAY>())
                    std::get<handler::mouse_axis>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_ENTER_DISPLAY])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_MOUSE_LEAVE_DISPLAY>())
                    std::get<handler::mouse_axis>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_LEAVE_DISPLAY])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;

                //  Joystick events
                case ALLEGRO_EVENT_JOYSTICK_AXIS:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_AXIS>())
                    std::get<handler::joystick_axis>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_AXIS])(
                        event.joystick.stick, event.joystick.axis, event.joystick.pos, event.joystick.id);
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_DOWN>())
                    std::get<handler::joystick_button>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_DOWN])(
                        event.joystick.button, event.joystick.id);
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_UP>())
                    std::get<handler::joystick_button>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_UP])(
                        event.joystick.button, event.joystick.id);
                    break;

                //  Touch events
                case ALLEGRO_EVENT_TOUCH_BEGIN:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_TOUCH_BEGIN>())
                    std::get<handler::touch>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_BEGIN])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_END:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_TOUCH_END>())
                    std::get<handler::touch>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_END])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_MOVE:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_TOUCH_MOVE>())
                    std::get<handler::touch>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_MOVE])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_CANCEL:
                    if constexpr (handlers<WTE_GAME_HANDLES>::is_set<WTE_EVENT_TOUCH_CANCEL>())
                    std::get<handler::touch>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_CANCEL])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                }
            else
                switch(event.type) {
                //  Keyboard events
                case ALLEGRO_EVENT_KEY_DOWN:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_KEY_DOWN>())
                    std::get<handler::key>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_KEY_DOWN])(
                        event.keyboard.keycode, event.keyboard.display);
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_KEY_UP>())
                    std::get<handler::key>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_KEY_UP])(
                        event.keyboard.keycode, event.keyboard.display);
                    break;

                //  Mouse events
                case ALLEGRO_EVENT_MOUSE_AXES:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_AXES>())
                    std::get<handler::mouse_axis>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_AXES])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_BUTTON_DOWN>())
                    std::get<handler::mouse_button>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_BUTTON_DOWN])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.button, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_BUTTON_UP>())
                    std::get<handler::mouse_button>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_BUTTON_UP])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.button, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_WARPED:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_WARPED>())
                    std::get<handler::mouse_axis>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_WARPED])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_ENTER_DISPLAY>())
                    std::get<handler::mouse_axis>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_ENTER_DISPLAY])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_MOUSE_LEAVE_DISPLAY>())
                    std::get<handler::mouse_axis>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_LEAVE_DISPLAY])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);
                    break;

                //  Joystick events
                case ALLEGRO_EVENT_JOYSTICK_AXIS:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_AXIS>())
                    std::get<handler::joystick_axis>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_AXIS])(
                        event.joystick.stick, event.joystick.axis, event.joystick.pos, event.joystick.id);
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_DOWN>())
                    std::get<handler::joystick_button>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_DOWN])(
                        event.joystick.button, event.joystick.id);
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_JOYSTICK_BUTTON_UP>())
                    std::get<handler::joystick_button>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_JOYSTICK_BUTTON_UP])(
                        event.joystick.button, event.joystick.id);
                    break;

                //  Touch events
                case ALLEGRO_EVENT_TOUCH_BEGIN:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_TOUCH_BEGIN>())
                    std::get<handler::touch>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_BEGIN])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_END:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_TOUCH_END>())
                    std::get<handler::touch>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_END])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_MOVE:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_TOUCH_MOVE>())
                    std::get<handler::touch>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_MOVE])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                case ALLEGRO_EVENT_TOUCH_CANCEL:
                    if constexpr (handlers<WTE_NONGAME_HANDLES>::is_set<WTE_EVENT_TOUCH_CANCEL>())
                    std::get<handler::touch>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_TOUCH_CANCEL])(
                        event.touch.id, event.touch.x, event.touch.y, event.touch.dx, event.touch.dy, event.touch.primary, event.touch.display);
                    break;
                }*/
        };

        static void create_event_queue(void);                 //  Create the input queue.
        static void destroy_event_queue(void);                //  Destroy the input queue.
        static void record_event(const ALLEGRO_EVENT&);       //  Record input events.
        static const bool save_recorder(void);                //  Save input events.
        static const bool check_events(void);                 //  Check the input queue for events.
        static void capture_states(const ALLEGRO_EVENT&);     //  Capture input states.
        static void run_game_handlers(const ALLEGRO_EVENT&);  //  Process in-game input events.

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
