/*!
 * WTEngine | File:  engine_time.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ENGINE_TIME_HPP
#define WTE_ENGINE_TIME_HPP

#include <cstdint>

namespace wte {

/*!
 * \class engine_time
 * \brief Have a manager extend this to track the game timer.
 */
class engine_time final {
    friend class engine;

    public:
        engine_time() = delete;
        ~engine_time() = delete;
        engine_time(const engine_time&) = delete;
        void operator=(engine_time const&) = delete;

        /*!
         * \brief Check the internal timer.
         * 
         * Call this member to check the game timer.
         * 
         * \return Timer value.
         */
        static const int64_t check_time(void);

    private:
        /*!
         * \brief Set the internal timer.
         * 
         * This is called once during the game loop in class wte_main
         * and when starting/stopping the game.
         * 
         * \param t Timer value.
         */
        static void set_time(
            const int64_t& t
        );

        //  Track game timer
        static int64_t current_time;
};

}  // end namespace wte

#endif
