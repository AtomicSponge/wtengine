/*!
 * WTEngine | File:  system.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_SYS_SYSTEM_HPP
#define WTE_SYS_SYSTEM_HPP

#include <string>
#include <memory>

#include "../mgr/message_manager.hpp"
#include "../mgr/entity_manager.hpp"
#include "../cmp/components.hpp"

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
        inline virtual ~system() {};

        //!  Remove copy constructor
        system(const system&) = delete;
        //!  Remove assignment operator
        void operator=(system const&) = delete;

        //!  Enable the system
        inline void enable(void);
        //!  Disable the system - Must override and define to be used
        inline virtual void disable(void) {};
        //!  Check if the system is enabled
        inline const bool is_enabled(void) const;

        //!  Get the system name
        inline const std::string get_name(void) const;

        //!  Override this to create custom run method
        virtual void run(mgr::entity_manager&,
                         mgr::message_manager&,
                         const int64_t&) = 0;

    protected:
        /*!
         * \brief System constructor.
         * 
         * \param n System name.
         */
        inline system(const std::string& n) : name(n), enabled(true) {};

        //!  Name of system.
        std::string name;
        //!  Flag to enable/disable system.
        bool enabled;
};

/*!
 * \typedef std::unique_ptr<system> system_uptr
 * System unique pointer.
 */
typedef std::unique_ptr<system> system_uptr;

} //  namespace sys

} //  namespace wte

#endif
