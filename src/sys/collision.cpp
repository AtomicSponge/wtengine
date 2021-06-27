/*!
 * WTEngine | File:  collision.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/collision.hpp"

namespace wte
{

namespace sys
{

colision::colision() : system("colision") {}
colision::~colision() {}

void colision::disable(void) { enabled = false; };

void colision::run(void) {
    const_component_container<cmp::team> team_components = mgr::entities::get_components<cmp::team>();

    for(auto & it_a : team_components) {
        for(auto & it_b : team_components) {
            /*
                * Only test if:  Not the same entity.
                *                Entities are on different teams.
                *                Both entities have a location.
                *                Both entities have a hitbox.
                *                Both entities are enabled.
                *                The entity being compared against is solid.
                */
            if((it_a.first != it_b.first) &&
                (it_a.second->get_team() != it_b.second->get_team()) &&
                mgr::entities::has_component<cmp::location>(it_a.first) &&
                mgr::entities::has_component<cmp::location>(it_b.first) &&
                mgr::entities::has_component<cmp::hitbox>(it_a.first) &&
                mgr::entities::has_component<cmp::hitbox>(it_b.first) &&
                mgr::entities::get_component<cmp::enabled>(it_a.first)->check() &&
                mgr::entities::get_component<cmp::enabled>(it_b.first)->check() &&
                mgr::entities::get_component<cmp::hitbox>(it_b.first)->is_solid())
            {
                //  Use AABB to test colision
                if((mgr::entities::get_component<cmp::location>(it_a.first)->get_x() <
                    mgr::entities::get_component<cmp::location>(it_b.first)->get_x() +
                    mgr::entities::get_component<cmp::hitbox>(it_b.first)->get_width()
                    && 
                    mgr::entities::get_component<cmp::location>(it_a.first)->get_x() +
                    mgr::entities::get_component<cmp::hitbox>(it_a.first)->get_width() >
                    mgr::entities::get_component<cmp::location>(it_b.first)->get_x())
                    &&
                    (mgr::entities::get_component<cmp::location>(it_a.first)->get_y() <
                    mgr::entities::get_component<cmp::location>(it_b.first)->get_y() +
                    mgr::entities::get_component<cmp::hitbox>(it_b.first)->get_height()
                    && 
                    mgr::entities::get_component<cmp::location>(it_a.first)->get_y() +
                    mgr::entities::get_component<cmp::hitbox>(it_a.first)->get_height() >
                    mgr::entities::get_component<cmp::location>(it_b.first)->get_y()))
                {
                    //  Send a message that two entities colided.
                    //  Each entity will get a colision message.
                    //  Ex:  A hit B, B hit A.
                    mgr::messages::add_message(message("entities",
                                                    mgr::entities::get_name(it_a.first),
                                                    mgr::entities::get_name(it_b.first),
                                                    "colision", ""));
                }
            } //  End skip self check
        } //  End it_b loop
    } //  End it_a loop
}

} //  namespace sys

} //  namespace wte
