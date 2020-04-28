/*!
 * \brief WTEngine | File:  engine_time.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine Time base class.
 */

#ifndef WTE_MGR_ENGINE_TIME_HPP
#define WTE_MGR_ENGINE_TIME_HPP

#include <atomic>

namespace wte
{

namespace mgr
{

//!  Engine Time class
/*!
 * Have a manager extend this to track the game timer.
 */
class engine_time {
    public:
        inline virtual ~engine_time() {};

        /*!
         * Set the internal timer.
         * This is called once during the game loop in class wte_main
         * and when starting/stopping the game.
         */
        inline static void set_time(const int64_t t) { current_time = t; };

    private:
        //  Track game timer
        static std::atomic<int64_t> current_time;

    protected:
        inline engine_time() {};

        //!  Check the internal timer.
        //!  Classes that extend this object can call this member to check the game timer.
        inline const int64_t check_time(void) const { return current_time; };
};

inline std::atomic<int64_t> engine_time::current_time = 0;

}  // end namespace mgr

}  // end namespace wte

#endif
