/*!
 * WTEngine | File:  animate.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/animate.hpp"

namespace wte
{

namespace sys
{

animate::animate() : system("animate") {}
animate::~animate() {}

void animate::run(mgr::entity_manager& world,
                mgr::message_manager& messages,
                const int64_t& current_time) {
    component_container<cmp::animator> animation_components = world.set_components<cmp::animator>();

    for(auto & it : animation_components) {
        if(world.has_component<cmp::visible>(it.first) &&
            world.get_component<cmp::visible>(it.first)->check())
                it.second->run(it.first, world, current_time);
    }
}

} //  namespace sys

} //  namespace wte
