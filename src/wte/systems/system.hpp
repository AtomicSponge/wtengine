/*
  WTEngine
  By:  Matthew Evans
  File:  system.hpp

  See LICENSE.txt for copyright information

  System Inferface Class
  Extend this to create a new system
*/

#ifndef WTE_SYS_SYSTEM_HPP
#define WTE_SYS_SYSTEM_HPP

#include <string>
#include <memory>

#include "../managers/message_manager.hpp"
#include "../managers/entity_manager.hpp"
#include "../components/components.hpp"

namespace wte
{

namespace sys
{

//! System interface class
/*!
  Extend this to create a new system
*/
class system {
    public:
        inline virtual ~system() {};

        //!  Remove copy constructor
        system(const system&) = delete;
        //!  Remove assignment operator
        void operator=(system const&) = delete;

        //!  Enable the system
        inline void enable(void) { enabled = true; };
        //!  Disable the system - Must override and define to be used
        inline virtual void disable(void) {};
        //!  Check if the system is enabled
        inline const bool is_enabled(void) const { return enabled; };

        //!  Get the system name
        inline const std::string get_name(void) const { return name; };

        //!  Override this to create custom run method
        virtual void run(mgr::entity_manager&, mgr::message_manager&, int64_t) = 0;
        //!  Override this for message processing.
        virtual void dispatch(mgr::entity_manager&, message_container) {};

    protected:
        inline system(const std::string n) : name(n), enabled(true) {};

        bool enabled;
        std::string name;
};

// System unique pointer
typedef std::unique_ptr<system> system_uptr;

} //  namespace sys

} //  namespace wte

#endif
