/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_INPUT_HPP
#define WTE_INPUT_HPP

#include <map>
#include <utility>
#include <functional>
#include <cmath>
#include <stdexcept>

#include <allegro5/allegro.h>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/menus.hpp"
#include "wtengine/config.hpp"

namespace wte {

/*!
 * \class input
 * \brief Manage the input queue and process events.
 */
class input {
    public:
        virtual ~input() = default;             //!<  Default virtual destructor.
        input(const input&) = delete;           //!<  Delete copy constructor.
        void operator=(input const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Toggle input recording.
         */
        static void toggle_input_recording(void);

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
                    inline static std::function<void(void)> up = [](){};       //!<  Player 1 button up on down event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 1 button down on down event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 1 button left on down event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 1 button right on down event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 1 button action1 on down event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 1 button action2 on down event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 1 button action3 on down event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 1 button action4 on down event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 1 button action5 on down event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 1 button action6 on down event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 1 button action7 on down event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 1 button action8 on down event.
                };

                /*!
                 * \struct onup
                 * \brief Player 1 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 1 button up on up event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 1 button down on up event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 1 button left on up event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 1 button right on up event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 1 button action1 on up event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 1 button action2 on up event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 1 button action3 on up event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 1 button action4 on up event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 1 button action5 on up event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 1 button action6 on up event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 1 button action7 on up event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 1 button action8 on up event.
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
                    inline static std::function<void(void)> up = [](){};       //!<  Player 2 button up on down event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 2 button down on down event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 2 button left on down event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 2 button right on down event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 2 button action1 on down event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 2 button action2 on down event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 2 button action3 on down event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 2 button action4 on down event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 2 button action5 on down event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 2 button action6 on down event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 2 button action7 on down event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 2 button action8 on down event.
                };
                
                /*!
                 * \struct onup
                 * \brief Player 2 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};       //!<  Player 2 button up on up event.
                    inline static std::function<void(void)> down = [](){};     //!<  Player 2 button down on up event.
                    inline static std::function<void(void)> left = [](){};     //!<  Player 2 button left on up event.
                    inline static std::function<void(void)> right = [](){};    //!<  Player 2 button right on up event.
                    inline static std::function<void(void)> action1 = [](){};  //!<  Player 2 button action1 on up event.
                    inline static std::function<void(void)> action2 = [](){};  //!<  Player 2 button action2 on up event.
                    inline static std::function<void(void)> action3 = [](){};  //!<  Player 2 button action3 on up event.
                    inline static std::function<void(void)> action4 = [](){};  //!<  Player 2 button action4 on up event.
                    inline static std::function<void(void)> action5 = [](){};  //!<  Player 2 button action5 on up event.
                    inline static std::function<void(void)> action6 = [](){};  //!<  Player 2 button action6 on up event.
                    inline static std::function<void(void)> action7 = [](){};  //!<  Player 2 button action7 on up event.
                    inline static std::function<void(void)> action8 = [](){};  //!<  Player 2 button action8 on up event.
                };
            };
        };

        /*!
         * \struct lastkeypress
         * \brief Record last key press.
         */
        struct lastkeypress {
            static const int64_t& timer;
            static const int& key;
        };

        /*!
         * \struct lastbuttonpress
         * \brief Record last button press.
         */
        struct lastbuttonpress {
            static const int64_t& timer;
            static const int& button;
        };

    protected:
        input();

        /*!
         * \brief Create the input queue for capturing events. 
         * Called during engine initialization.
         */
        void create_input_event_queue(void);

        /*!
         * \brief Destroy the input queue.
         * Called during engine de-initialization.
         */
        void destroy_input_event_queue(void);

        /*!
         * \brief Check the input queue for events.
         * Called during the main engine loop.
         * Passes any events to the input handler.
         */
        void check_input_events(void);

    private:
        struct _lastkeypress {
            inline static int64_t timer = 0;
            inline static int key = 0;
        };

        struct _lastbuttonpress {
            inline static int64_t timer = 0;
            inline static int button = 0;
        };

        void handle_input_event(const ALLEGRO_EVENT&);  //  Process input events

        ALLEGRO_EVENT_QUEUE* input_event_queue;  //  Input event queue.
        static bool initialized;  //  Restrict to one instance.
};

} //  end namespace wte

#endif
