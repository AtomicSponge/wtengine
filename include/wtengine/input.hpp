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
            switch(event.type) {
            //  Keyboard events
            case ALLEGRO_EVENT_KEY_DOWN:
                if constexpr (_global_hreg[WTE_EVENT_KEY_DOWN] == WTE_HANDLER_SET)
                    std::get<handler::key>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_KEY_DOWN])(
                        event.keyboard.keycode, event.keyboard.display);
                break;
            case ALLEGRO_EVENT_KEY_UP:
                if constexpr (_global_hreg[WTE_EVENT_KEY_UP] == WTE_HANDLER_SET)
                    std::get<handler::key>(handlers<WTE_GLOBAL_HANDLES>::_handlers[WTE_EVENT_KEY_UP])(
                        event.keyboard.keycode, event.keyboard.display);
                break;

            //  Mouse events
            case ALLEGRO_EVENT_MOUSE_AXES:
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                break;
            case ALLEGRO_EVENT_MOUSE_WARPED:
                break;
            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                break;
            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                break;

            //  Joystick events
            case ALLEGRO_EVENT_JOYSTICK_AXIS:
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                break;
            case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                break;

            //  Touch events
            case ALLEGRO_EVENT_TOUCH_BEGIN:
                break;
            case ALLEGRO_EVENT_TOUCH_END:
                break;
            case ALLEGRO_EVENT_TOUCH_MOVE:
                break;
            case ALLEGRO_EVENT_TOUCH_CANCEL:
                break;
            }
            if(config::flags::game_started)
                switch(event.type) {
                //  Keyboard events
                case ALLEGRO_EVENT_KEY_DOWN:
                    if constexpr (_game_hreg[WTE_EVENT_KEY_DOWN] == WTE_HANDLER_SET)
                        std::get<handler::key>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_KEY_DOWN])(
                            event.keyboard.keycode, event.keyboard.display);
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if constexpr (_game_hreg[WTE_EVENT_KEY_UP] == WTE_HANDLER_SET)
                        std::get<handler::key>(handlers<WTE_GAME_HANDLES>::_handlers[WTE_EVENT_KEY_UP])(
                            event.keyboard.keycode, event.keyboard.display);
                    break;

                //  Mouse events
                case ALLEGRO_EVENT_MOUSE_AXES:
                    //if(std::holds_alternative<handler::mouse_axes>(handlers<GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_AXES]))
                    /*std::get<handler::mouse_axes>(handlers<GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_AXES])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);*/
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    break;
                case ALLEGRO_EVENT_MOUSE_WARPED:
                    /*std::get<handler::mouse_warped>(handlers<GAME_HANDLES>::_handlers[WTE_EVENT_MOUSE_WARPED])(
                        event.mouse.x, event.mouse.y, event.mouse.z, event.mouse.w,
                        event.mouse.dx, event.mouse.dy, event.mouse.dz, event.mouse.dw,
                        event.mouse.pressure, event.mouse.display);*/
                    break;
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    break;

                //  Joystick events
                case ALLEGRO_EVENT_JOYSTICK_AXIS:
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                    break;

                //  Touch events
                case ALLEGRO_EVENT_TOUCH_BEGIN:
                    break;
                case ALLEGRO_EVENT_TOUCH_END:
                    break;
                case ALLEGRO_EVENT_TOUCH_MOVE:
                    break;
                case ALLEGRO_EVENT_TOUCH_CANCEL:
                    break;
                }
            else
                switch(event.type) {
                //  Keyboard events
                case ALLEGRO_EVENT_KEY_DOWN:
                    if constexpr (_nongame_hreg[WTE_EVENT_KEY_DOWN])
                        std::get<handler::key>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_KEY_DOWN])(
                            event.keyboard.keycode, event.keyboard.display);
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if constexpr (_nongame_hreg[WTE_EVENT_KEY_UP])
                        std::get<handler::key>(handlers<WTE_NONGAME_HANDLES>::_handlers[WTE_EVENT_KEY_UP])(
                            event.keyboard.keycode, event.keyboard.display);
                    break;

                //  Mouse events
                case ALLEGRO_EVENT_MOUSE_AXES:
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                    break;
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    break;
                case ALLEGRO_EVENT_MOUSE_WARPED:
                    break;
                case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    break;
                case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
                    break;

                //  Joystick events
                case ALLEGRO_EVENT_JOYSTICK_AXIS:
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
                    break;
                case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
                    break;

                //  Touch events
                case ALLEGRO_EVENT_TOUCH_BEGIN:
                    break;
                case ALLEGRO_EVENT_TOUCH_END:
                    break;
                case ALLEGRO_EVENT_TOUCH_MOVE:
                    break;
                case ALLEGRO_EVENT_TOUCH_CANCEL:
                    break;
                }
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
