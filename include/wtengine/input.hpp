/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
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

#include "wtengine/config.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/input_inf.hpp"

namespace wte
{

/*!
 * \class input
 * \brief Manage the input queue and process events.
 */
class input {
    public:
        /*!
         * \brief WTE Input destructor.
         */
        inline virtual ~input() {};

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
        /*!
         * \brief WTE Input constructor.
         */
        input();

        /*!
         * \brief Create the input queue for capturing events.
         * 
         * Called during engine initialization.
         */
        void create_input_event_queue(void);

        /*!
         * \brief Destroy the input queue.
         * 
         * Called during engine de-initialization.
         */
        void destroy_input_event_queue(void);

        /*!
         * \brief Check the input queue for events.
         * 
         * Called during the main engine loop.  Pass any events to the input handler.
         */
        void check_input_events(void);

        struct lastkeypress {
            inline static int64_t timer = 0;
            inline static int key = 0;
        };

        struct lastbuttonpress {
            inline static int64_t timer = 0;
            inline static int button = 0;
        };

    private:
        void handle_input_event(const ALLEGRO_EVENT&);

        ALLEGRO_EVENT_QUEUE* input_event_queue;

        mgr::input_inf menu_inf;
};

} //  end namespace wte

#endif
