/*
  WTEngine
  By:  Matthew Evans
  File:  input.hpp

  See LICENSE.txt for copyright information

  Input system
  Override this to create a custom input system
*/

#ifndef WTE_SYS_INPUT_HPP
#define WTE_SYS_INPUT_HPP

#include "system.hpp"

namespace wte
{

namespace sys
{

//! Input system
/*!
  Processes keyboard input
*/
class input : public system {
    public:
        inline input() : system("input") {};
        inline ~input() {};

        inline void disable(void) { enabled = false; };

        //! Input system run method
        inline void run(mgr::entity_manager&, mgr::message_manager&, int64_t);

    protected:
        component_container input_components;
        //!  Override this to implement input handling
        virtual void custom_run(mgr::entity_manager&, mgr::message_manager&) {};
};

/*!
  Get all entities tagged with the input_handler component and store for processing
*/
inline void input::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    //  Find the entities with the input handler component
    input_components = world.get_components<cmp::input_handler>();

    //  Run custom input handler
    custom_run(world, messages);
}

} //  namespace sys

} //  namespace wte

#endif
