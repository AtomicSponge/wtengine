/*!
 * WTEngine | File:  movement.cpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/movement.hpp"

namespace wte
{

namespace sys
{

movement::movement() : system("movement") {}
movement::~movement() {}

void movement::run(mgr::entity_manager& world,
                   mgr::message_manager& messages,
                   const int64_t& current_time) {
    //  Find the entities with a velocity component.
    const_component_container<cmp::velocity> vel_components = world.get_components<cmp::velocity>();
    for(auto & it : vel_components) {
        if(world.has_component<cmp::location>(it.first) &&
            world.has_component<cmp::direction>(it.first))
        {
            world.set_component<cmp::location>(it.first)->adjust_x(
                it.second->get_x_vel() *
                std::cos(world.get_component<cmp::direction>(it.first)->get_radians())
            );

            world.set_component<cmp::location>(it.first)->adjust_y(
                it.second->get_y_vel() *
                std::sin(world.get_component<cmp::direction>(it.first)->get_radians())
            );
        }
    }

    //  Now check all bounding boxes.
    const_component_container<cmp::bounding_box> bbox_components = world.get_components<cmp::bounding_box>();
    for(auto & it : bbox_components) {
        if(world.has_component<cmp::location>(it.first)) {
            if(world.get_component<cmp::location>(it.first)->get_x() < it.second->get_min_x())
                world.set_component<cmp::location>(it.first)->set_x(it.second->get_min_x());
            else if(world.get_component<cmp::location>(it.first)->get_x() > it.second->get_max_x())
                world.set_component<cmp::location>(it.first)->set_x(it.second->get_max_x());

            if(world.get_component<cmp::location>(it.first)->get_y() < it.second->get_min_y())
                world.set_component<cmp::location>(it.first)->set_y(it.second->get_min_y());
            else if(world.get_component<cmp::location>(it.first)->get_y() > it.second->get_max_y())
                world.set_component<cmp::location>(it.first)->set_y(it.second->get_max_y());
        }
    }
}

} //  namespace sys

} //  namespace wte
