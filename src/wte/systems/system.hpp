/*
  WTEngine
  By:  Matthew Evans
  File:  system.hpp

  See LICENSE.txt for copyright information

  System Inferface Class
  Extend this to create a new system
*/

#ifndef WTE_ECS_SYSTEM_HPP
#define WTE_ECS_SYSTEM_HPP

#include <string>
#include <memory>

#include "../message_queue.hpp"
#include "../entity_manager.hpp"
#include "../components/components.hpp"

namespace wte
{

namespace ecs
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

        inline const std::string get_name(void) const { return name; };

        //  Override this to create custom run method
        virtual void run(entity_manager&, msg::message_queue&, int64_t) = 0;
        //  Override this for message processing.
        virtual void dispatch(entity_manager&, msg::message_container) {};

    protected:
        inline system() {};

        std::string name;
};

//! System unique pointer
typedef std::unique_ptr<system> system_uptr;

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
