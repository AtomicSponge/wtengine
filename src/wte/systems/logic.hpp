/*
  WTEngine
  By:  Matthew Evans
  File:  logic.hpp

  See LICENSE.txt for copyright information

  Logic system
  Override this to define custom ai
*/

#ifndef WTE_ECS_SYSTEM_LOGIC_HPP
#define WTE_ECS_SYSTEM_LOGIC_HPP

#include "system.hpp"

namespace wte
{

namespace ecs
{

namespace sys
{

//! Logic system
/*!
  Processes entities that have ai components
*/
class logic : public system {
    public:
        void run(entity_manager&, msg::message_queue&, int64_t);    /*!< Run the logic system */
        void dispatch(entity_manager&, msg::message_queue&);        /*!< Dispatch logic messages */

    protected:
        component_container ai_components;  /*!< Container for logic components */

        virtual void custom_run(entity_manager&, msg::message_queue&, int64_t) {};  /*!< Override to define behaviour */
        virtual void process_messages(entity_manager&, msg::message_container) {};  /*!< Override to define behaviour */
};

//! Run the logic system
/*!
  Finds all entities with an ai component and processes their logic
  Override the custom_run member to define behaviour
*/
inline void logic::run(entity_manager& world, msg::message_queue& messages, int64_t current_time) {
    //  Find the entities with the input handler component
    ai_components = world.get_components<cmp::ai>();

    //  Run custom input handler
    custom_run(world, messages, current_time);
}

//! Logic dispatch member
/*!
  Get logic messages for processing
  Override the process_messages member to define behaviour
*/
inline void logic::dispatch(entity_manager& world, msg::message_queue& messages) {
    msg::message_container spawner_messages = messages.get_messages("logic");
    process_messages(world, spawner_messages);
}

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
