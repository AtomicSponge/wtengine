/*!
 * WTEngine | File:  system.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_SYSTEM_HPP
#define WTE_SYS_SYSTEM_HPP

#include <string>
#include <memory>

#include "wtengine/cmp/_components.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/entities.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class system
 * \brief Interface class for creating systems.
 */
class system {
    public:
        virtual ~system() {};

        //!  Remove copy constructor
        system(const system&) = delete;
        //!  Remove assignment operator
        void operator=(system const&) = delete;

        //!  Get the system name
        inline const std::string get_name(void) const { return name; };

        //!  Override this to create custom run method
        virtual void run(void) = 0;

    protected:
        /*!
         * \brief System constructor.
         * 
         * \param n System name.
         */
        system(const std::string& n) : name(n) {};

        //!  Name of system.
        const std::string name;
};

/*!
 * \typedef std::unique_ptr<system> system_uptr
 * System unique pointer.
 */
typedef std::unique_ptr<system> system_uptr;

} //  namespace sys

} //  namespace wte

#endif
