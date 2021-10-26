/*!
 * WTEngine | File:  system.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_SYSTEM_HPP
#define WTE_SYS_SYSTEM_HPP

#include <string>
#include <memory>

#include "wtengine/cmp/_components.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte::sys {

/*!
 * \class system
 * \brief Interface class for creating Systems.
 */
class system {
    public:
        virtual ~system() = default;             //!<  Default virtual destructor.
        system(const system&) = delete;          //!<  Delete copy constructor.
        void operator=(system const&) = delete;  //!<  Delete assignment operator.

        //!  Override this to create custom System run method.
        virtual void run(void) = 0;

        const std::string name;  //!<  System name.
        const bool timed;        //!<  Bind system to timer.

    protected:
        /*!
         * \brief Create a new System object.
         * \param n System name.
         * \param t Flag to bind to timer.
         */
        system(
            const std::string& n,
            const bool& t
        ) : name(n), timed(t) {};
};

/*!
 * \typedef std::unique_ptr<system> system_uptr
 * System unique pointer.
 */
typedef std::unique_ptr<system> system_uptr;

}  //  end namespace wte::sys

#endif
