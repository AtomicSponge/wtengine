/*!
 * WTEngine | File:  logic.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include "wtengine/logic.hpp"

namespace wte
{

namespace sys
{

logic::logic() : system("logic") {};
logic::~logic() {};

void logic::run(mgr::entity_manager& world,
                mgr::message_manager& messages,
                const int64_t& current_time) override {
    //  Find the entities with the input handler component
    component_container<cmp::ai> ai_components = world.set_components<cmp::ai>();

    for(auto & it : ai_components) {
        //  Make sure there's an enabled component
        if(world.has_component<cmp::enabled>(it.first)) {
            //  Process enabled or disabled ai
            if(world.get_component<cmp::enabled>(it.first)->check())
                it.second->run_enabled(it.first, world, messages, current_time);
            else
                it.second->run_disabled(it.first, world, messages, current_time);
        }
    }
}

} //  namespace sys

} //  namespace wte
