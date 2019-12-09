/*
  WTEngine
  By:  Matthew Evans
  File:  spawner.hpp

  See LICENSE.txt for copyright information

  System for creating new entities during runtime
  Override this to define custom behaviour
*/

#ifndef WTE_ECS_SYSTEM_SPAWNER_HPP
#define WTE_ECS_SYSTEM_SPAWNER_HPP

#include "../wte_globals.hpp"
#include "system.hpp"

namespace wte
{

namespace ecs
{

namespace sys
{

//! Spawner system
/*!
  Handles the creation and deletion of entities
  Override to define custom entity creation
*/
class spawner : public system {
    public:
        inline void run(entity_manager&, msg::message_queue&, int64_t);
        inline void dispatch(entity_manager&, msg::message_queue&);
    
    private:
        void process_deletes(entity_manager&, msg::message_container);

    protected:
        virtual void process_spawns(entity_manager&, msg::message_container) {};
};


//! Run the spawner system
/*!
  Empty run member
*/
inline void spawner::run(entity_manager& world, msg::message_queue& messages, int64_t current_time) {
    //
}

//! Spawner dispatch member
/*!
  Split out the spawn and delete messages, pass them to each corresponding member for processing
*/
inline void spawner::dispatch(entity_manager& world, msg::message_queue& messages) {
    msg::message_container spawner_messages = messages.get_messages("spawn");
    process_spawns(world, spawner_messages);

    msg::message_container delete_messages = messages.get_messages("delete");
    process_deletes(world, delete_messages);
}

//! Spawner process deletes member
/*!
  Deletes entities based on the to field of the message
  Checks entites by name and deletes if found
*/
inline void spawner::process_deletes(entity_manager& world, msg::message_container messages) {
    for(msg::message_iterator m_it = messages.begin(); m_it != messages.end(); m_it++) {
        ecs::component_container name_components = world.get_components<cmp::name>();
        
        //  Check all named entities and delete if it exists
        for(ecs::component_iterator c_it = name_components.begin(); c_it != name_components.end(); c_it++) {
            if(m_it->get_to() == dynamic_cast<cmp::name*>(c_it->second.get())->name) {
                world.delete_entity(c_it->first);
            }
        }
    }
}

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
