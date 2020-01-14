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
        inline spawner() { name = "spawner"; };
        inline ~spawner() {};

        inline void run(entity_manager&, msg::message_queue&, int64_t);
        inline void dispatch(entity_manager&, msg::message_container);

    protected:
        virtual void process_spawn(entity_manager&, msg::arg_list) {};
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
inline void spawner::dispatch(entity_manager& world, msg::message_container messages) {
    for(msg::message_iterator it = messages.begin(); it != messages.end(); it++) {
        if(it->get_cmd() == "new") process_spawn(world, it->get_split_args());

        if(it->get_cmd() == "delete") {
            ecs::component_container name_components = world.get_components<cmp::name>();
        
            //  Check all named entities and delete if it exists
            for(ecs::component_iterator c_it = name_components.begin(); c_it != name_components.end(); c_it++) {
                if(it->get_args() == dynamic_cast<cmp::name*>(c_it->second.get())->name) {
                    world.delete_entity(c_it->first);
                }
            }
        }
    }
}

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
