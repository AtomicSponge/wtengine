/*!
 * WTEngine | File:  engine_time.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ENGINE_TIME_HPP
#define WTE_ENGINE_TIME_HPP

#include <atomic>

namespace wte
{

/*!
 * \class engine_time
 * \brief Have a manager extend this to track the game timer.
 */
class engine_time {
    friend class engine;

    public:
        /*!
         * \brief Check the internal timer.
         * 
         * Call this member to check the game timer.
         * 
         * \return Timer value.
         */
        static const int64_t check_time(void) {
            return current_time.load(std::memory_order_acquire);
        };

    private:
        engine_time() {};
        ~engine_time() {};

        /*!
         * \brief Set the internal timer.
         * 
         * This is called once during the game loop in class wte_main
         * and when starting/stopping the game.
         * 
         * \param t Timer value.
         */
        static void set_time(const int64_t& t) {
            current_time.store(t, std::memory_order_release);
        };

        //  Track game timer
        inline static std::atomic<int64_t> current_time = 0;

        friend class wte_main;
};

}  // end namespace wte

#endif
