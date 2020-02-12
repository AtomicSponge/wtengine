/*
  WTEngine
  By:  Matthew Evans
  File:  engine_time.hpp

  See LICENSE.txt for copyright information

  Engine Time base class
*/

#ifndef WTE_MGR_ENGINE_TIME_HPP
#define WTE_MGR_ENGINE_TIME_HPP

namespace wte
{

namespace mgr
{

//!  Engine Time class
/*!
  Have a manager extend this to have access to check the internal timer value
*/
class engine_time {
    public:
        inline virtual ~engine_time() {};

        //!  Set the internal timer for the manager
        inline static void set_time(const int64_t t) { current_time = t; };

    private:
        //  Track game timer
        static int64_t current_time;

    protected:
        inline engine_time() {};

        //!  Check the manager's internal timer
        inline const int64_t check_time(void) const { return current_time; };
};

inline int64_t engine_time::current_time = 0;

}  // end namespace mgr

}  // end namespace wte

#endif
