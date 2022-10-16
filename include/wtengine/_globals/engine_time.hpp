/*!
 * wtengine | File:  engine_time.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_ENGINE_TIME_HPP
#define WTE_ENGINE_TIME_HPP

#include <cstdint>

namespace wte {

/*!
 * \class engine_time
 * \brief Tracks current time of the engine.
 */
class engine_time final {
    friend class engine;

    public:
        engine_time() = delete;                       //!<  Delete constructor.
        ~engine_time() = delete;                      //!<  Delete destructor.
        engine_time(const engine_time&) = delete;     //!<  Delete copy constructor.
        void operator=(engine_time const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Check the internal engine timer.
         * \return Timer value.
         */
        static int64_t check(void);

    private:
        //  Sets the internal timer. Called internally by engine.
        static void set(const int64_t& t);
        static int64_t current_time;  //  Track game timer
};

}  //  end namespace wte

#endif
