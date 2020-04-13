/*!
 * \brief WTEngine | File:  spawner.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details System for creating new entities during runtime.
 * Override this to define custom behaviour.
 */

#ifndef WTE_SYS_SPAWNER_HPP
#define WTE_SYS_SPAWNER_HPP

#include "system.hpp"

namespace wte
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
        inline spawner() : system("spawner") {};
        inline ~spawner() {};

        //! Run the spawner system
        inline void run(mgr::entity_manager&, mgr::message_manager&, int64_t);
        //! Spawner dispatch member
        inline void dispatch(mgr::entity_manager&, message_container);

    protected:
        //!  Override to process messages and create entities
        virtual void process_spawn(mgr::entity_manager&, msg_arg_list) {};
};

/*!
  Empty run member
*/
inline void spawner::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    //
}

/*!
  Split out the spawn and delete messages, pass them to each corresponding member for processing
*/
inline void spawner::dispatch(mgr::entity_manager& world, message_container messages) {
    for(auto it = messages.begin(); it != messages.end(); it++) {
        if(it->get_cmd() == "new") process_spawn(world, it->get_arglist());

        if(it->get_cmd() == "delete") {
            component_container name_components = world.get_components<cmp::name>();
        
            //  Check all named entities and delete if it exists
            for(component_iterator c_it = name_components.begin(); c_it != name_components.end(); c_it++) {
                if(it->get_arg(0) == dynamic_cast<cmp::name*>(c_it->second.get())->name_str) {
                    world.delete_entity(c_it->first);
                }
            }
        }
    }
}

} //  namespace sys

} //  namespace wte

#endif
