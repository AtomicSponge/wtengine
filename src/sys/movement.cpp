/*!
 * WTEngine | File:  movement.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
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

void movement::run(void) {
    //  Find the entities with a velocity component.
    const_component_container<cmp::velocity> vel_components = mgr::entities::get_components<cmp::velocity>();
    for(auto & it : vel_components) {
        if(mgr::entities::has_component<cmp::location>(it.first) &&
            mgr::entities::has_component<cmp::direction>(it.first))
        {
            mgr::entities::set_component<cmp::location>(it.first)->adjust_x(
                it.second->get_x_vel() *
                std::cos(mgr::entities::get_component<cmp::direction>(it.first)->get_radians())
            );

            mgr::entities::set_component<cmp::location>(it.first)->adjust_y(
                it.second->get_y_vel() *
                std::sin(mgr::entities::get_component<cmp::direction>(it.first)->get_radians())
            );
        }
    }

    //  Now check all bounding boxes.
    const_component_container<cmp::bounding_box> bbox_components = mgr::entities::get_components<cmp::bounding_box>();
    for(auto & it : bbox_components) {
        if(mgr::entities::has_component<cmp::location>(it.first)) {
            if(mgr::entities::get_component<cmp::location>(it.first)->get_x() < it.second->get_min_x())
                mgr::entities::set_component<cmp::location>(it.first)->set_x(it.second->get_min_x());
            else if(mgr::entities::get_component<cmp::location>(it.first)->get_x() > it.second->get_max_x())
                mgr::entities::set_component<cmp::location>(it.first)->set_x(it.second->get_max_x());

            if(mgr::entities::get_component<cmp::location>(it.first)->get_y() < it.second->get_min_y())
                mgr::entities::set_component<cmp::location>(it.first)->set_y(it.second->get_min_y());
            else if(mgr::entities::get_component<cmp::location>(it.first)->get_y() > it.second->get_max_y())
                mgr::entities::set_component<cmp::location>(it.first)->set_y(it.second->get_max_y());
        }
    }
}

} //  namespace sys

} //  namespace wte
