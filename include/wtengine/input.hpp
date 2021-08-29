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
        virtual ~input() = default;
        input(const input&) = delete;
        void operator=(input const&) = delete;

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
                    inline static std::function<void(void)> up = [](){};
                    inline static std::function<void(void)> down = [](){};
                    inline static std::function<void(void)> left = [](){};
                    inline static std::function<void(void)> right = [](){};
                    inline static std::function<void(void)> action1 = [](){};
                    inline static std::function<void(void)> action2 = [](){};
                    inline static std::function<void(void)> action3 = [](){};
                    inline static std::function<void(void)> action4 = [](){};
                    inline static std::function<void(void)> action5 = [](){};
                    inline static std::function<void(void)> action6 = [](){};
                };

                /*!
                 * \struct onup
                 * \brief Player 1 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};
                    inline static std::function<void(void)> down = [](){};
                    inline static std::function<void(void)> left = [](){};
                    inline static std::function<void(void)> right = [](){};
                    inline static std::function<void(void)> action1 = [](){};
                    inline static std::function<void(void)> action2 = [](){};
                    inline static std::function<void(void)> action3 = [](){};
                    inline static std::function<void(void)> action4 = [](){};
                    inline static std::function<void(void)> action5 = [](){};
                    inline static std::function<void(void)> action6 = [](){};
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
                    inline static std::function<void(void)> up = [](){};
                    inline static std::function<void(void)> down = [](){};
                    inline static std::function<void(void)> left = [](){};
                    inline static std::function<void(void)> right = [](){};
                    inline static std::function<void(void)> action1 = [](){};
                    inline static std::function<void(void)> action2 = [](){};
                    inline static std::function<void(void)> action3 = [](){};
                    inline static std::function<void(void)> action4 = [](){};
                    inline static std::function<void(void)> action5 = [](){};
                    inline static std::function<void(void)> action6 = [](){};
                };
                
                /*!
                 * \struct onup
                 * \brief Player 2 on up events.
                 */
                struct onup {
                    inline static std::function<void(void)> up = [](){};
                    inline static std::function<void(void)> down = [](){};
                    inline static std::function<void(void)> left = [](){};
                    inline static std::function<void(void)> right = [](){};
                    inline static std::function<void(void)> action1 = [](){};
                    inline static std::function<void(void)> action2 = [](){};
                    inline static std::function<void(void)> action3 = [](){};
                    inline static std::function<void(void)> action4 = [](){};
                    inline static std::function<void(void)> action5 = [](){};
                    inline static std::function<void(void)> action6 = [](){};
                };
            };
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

        /*!
         * \struct lastkeypress
         * \brief Record last key press.
         */
        struct lastkeypress {
            inline static int64_t timer = 0;
            inline static int key = 0;
        };

        /*!
         * \struct lastbuttonpress
         * \brief Record last button press.
         */
        struct lastbuttonpress {
            inline static int64_t timer = 0;
            inline static int button = 0;
        };

    private:
        void handle_input_event(const ALLEGRO_EVENT&);  //  Process input events

        ALLEGRO_EVENT_QUEUE* input_event_queue;  //  Input event queue.
        static bool initialized;  //  Restrict to one instance.
};

} //  end namespace wte

#endif
