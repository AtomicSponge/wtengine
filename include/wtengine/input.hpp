/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
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
#include "wtengine/_globals/wte_exception.hpp"

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

        struct event {
            struct p1 {
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

            struct p2 {
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

    private:
        void handle_input_event(const ALLEGRO_EVENT&);

        ALLEGRO_EVENT_QUEUE* input_event_queue;
};

} //  end namespace wte

#endif
