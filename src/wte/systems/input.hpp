/*
  WTEngine
  By:  Matthew Evans
  File:  input.hpp

  See LICENSE.txt for copyright information

  Input system
  Override this to create a custom input system
*/

#ifndef WTE_ECS_SYSTEM_INPUT_HPP
#define WTE_ECS_SYSTEM_INPUT_HPP

#include "system.hpp"

namespace wte
{

namespace ecs
{

namespace sys
{

//! Input system
/*!
  Processes keyboard input
*/
class input : public system {
    public:
        inline void run(entity_manager&, msg::message_queue&, int64_t);

    protected:
        component_container input_components;
        virtual void custom_run(entity_manager&, msg::message_queue&) {};
};

//! Input system run method
/*!
  Get all entities tagged with the input_handler component and store for processing
*/
inline void input::run(entity_manager& world, msg::message_queue& messages, int64_t current_time) {
    //  Find the entities with the input handler component
    input_components = world.get_components<cmp::input_handler>();

    //  Run custom input handler
    custom_run(world, messages);
}

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
