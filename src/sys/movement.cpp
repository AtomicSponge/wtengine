/*!
 * WTEngine | File:  movement.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/movement.hpp"

namespace wte
{

namespace sys
{

/*
 *
 */
movement::movement() : system("movement") {}

/*
 *
 */
void movement::run(void) {
    //  Find the entities with a motion component.
    const_component_container<cmp::motion> vel_components = mgr::world::get_components<cmp::motion>();
    for(auto& it: vel_components) {
        try {
            //component_sptr
            //component_csptr

            mgr::world::set_component<cmp::location>(it.first)->adjust_x(
                it.second->get_x_vel() *
                std::cos(mgr::world::get_component<cmp::motion>(it.first)->get_direction())
            );

            mgr::world::set_component<cmp::location>(it.first)->adjust_y(
                it.second->get_y_vel() *
                std::sin(mgr::world::get_component<cmp::motion>(it.first)->get_direction())
            );
        } catch(...) { throw; }
    }

    //  Now check all bounding boxes.
    const_component_container<cmp::bounding_box> bbox_components = mgr::world::get_components<cmp::bounding_box>();
    for(auto& it: bbox_components) {
        try{
            //component_sptr
            //component_csptr

            if(mgr::world::get_component<cmp::location>(it.first)->get_x() < it.second->get_min_x())
                mgr::world::set_component<cmp::location>(it.first)->set_x(it.second->get_min_x());
            else if(mgr::world::get_component<cmp::location>(it.first)->get_x() > it.second->get_max_x())
                mgr::world::set_component<cmp::location>(it.first)->set_x(it.second->get_max_x());

            if(mgr::world::get_component<cmp::location>(it.first)->get_y() < it.second->get_min_y())
                mgr::world::set_component<cmp::location>(it.first)->set_y(it.second->get_min_y());
            else if(mgr::world::get_component<cmp::location>(it.first)->get_y() > it.second->get_max_y())
                mgr::world::set_component<cmp::location>(it.first)->set_y(it.second->get_max_y());
        } catch(...) { throw; }
    }
}

} //  namespace sys

} //  namespace wte
