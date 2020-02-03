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

        system(const system&) = delete;
        void operator=(system const&) = delete;

        inline const void enable(void) { enabled = true; };
        inline const void disable(void) { enabled = false; };
        inline const bool is_enabled(void) const { return enabled; };

        inline const std::string get_name(void) const { return name; };

        //  Override this to create custom run method
        virtual void run(mgr::entity_manager&, mgr::message_manager&, int64_t) = 0;
        //  Override this for message processing.
        virtual void dispatch(mgr::entity_manager&, message_container) {};

    protected:
        inline system() : enabled(true) {};

        bool enabled;
        std::string name;
};

//! System unique pointer
typedef std::unique_ptr<sys::system> system_uptr;

} //  namespace sys

} //  namespace wte

#endif
