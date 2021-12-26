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
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/config.hpp"
#include "wtengine/handlers.hpp"

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
         * \brief Reset P1 joystick polls.
         */
        static void reset_p1_pols(void);

        /*!
         * \brief Reset P2 joystick polls.
         */
        static void reset_p2_pols(void);

        /*!
         * \brief Reset P1 & P2 joystick polls.
         */
        static void reset_pols(void);

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
         * \struct event
         * \brief Input event macros.
         */
        struct event {
            /*!
             * \struct p1
             * \brief Player 1 input events.
             */
            struct p1 {
                /*!
                 * \struct ondown
                 * \brief Player 1 on down events.
                 */
                struct ondown {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 1 input up on down event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 1 input down on down event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 1 input left on down event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 1 input right on down event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 1 input action1 on down event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 1 input action2 on down event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 1 input action3 on down event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 1 input action4 on down event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 1 input action5 on down event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 1 input action6 on down event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 1 input action7 on down event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 1 input action8 on down event.
                };

                /*!
                 * \struct onup
                 * \brief Player 1 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 1 input up on up event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 1 input down on up event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 1 input left on up event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 1 input right on up event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 1 input action1 on up event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 1 input action2 on up event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 1 input action3 on up event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 1 input action4 on up event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 1 input action5 on up event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 1 input action6 on up event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 1 input action7 on up event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 1 input action8 on up event.
                };
            };

            /*!
             * \struct p2
             * \brief Player 2 input events.
             */
            struct p2 {
                /*!
                 * \struct ondown
                 * \brief Player 2 on down events.
                 */
                struct ondown {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 2 input up on down event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 2 input down on down event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 2 input left on down event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 2 input right on down event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 2 input action1 on down event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 2 input action2 on down event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 2 input action3 on down event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 2 input action4 on down event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 2 input action5 on down event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 2 input action6 on down event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 2 input action7 on down event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 2 input action8 on down event.
                };

                /*!
                 * \struct onup
                 * \brief Player 2 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 2 input up on up event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 2 input down on up event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 2 input left on up event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 2 input right on up event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 2 input action1 on up event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 2 input action2 on up event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 2 input action3 on up event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 2 input action4 on up event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 2 input action5 on up event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 2 input action6 on up event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 2 input action7 on up event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 2 input action8 on up event.
                };
            };
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
