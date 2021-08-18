/*!
 * WTEngine | File:  collision.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/collision.hpp"

namespace wte
{

namespace sys
{

/*
 *
 */
colision::colision() : system("colision") {}

/*
 *
 */
void colision::run(void) {
    const_component_container<cmp::team> team_components = mgr::world::get_components<cmp::team>();

    for(auto& it_a: team_components) {
        for(auto& it_b: team_components) {
            try {
                cmp::const_comp_ptr<cmp::location> temp_location_a =
                    mgr::world::get_component<cmp::location>(it_a.first);
                cmp::const_comp_ptr<cmp::location> temp_location_b =
                    mgr::world::get_component<cmp::location>(it_b.first);
                cmp::const_comp_ptr<cmp::hitbox> temp_hitbox_a =
                    mgr::world::get_component<cmp::hitbox>(it_a.first);
                cmp::const_comp_ptr<cmp::hitbox> temp_hitbox_b =
                    mgr::world::get_component<cmp::hitbox>(it_b.first);

                /*
                 * Only test if:  Not the same entity.
                 *                Entities are on different teams.
                 *                Both entities are solid.
                 */
                if(
                    it_a.first != it_b.first &&
                    it_a.second->get_team() != it_b.second->get_team() &&
                    temp_hitbox_a->is_solid() && temp_hitbox_b->is_solid()
                ) {
                    //  Use AABB to test colision
                    if(
                        temp_location_a->get_x() < temp_location_b->get_x() + temp_hitbox_b->get_width()
                        && 
                        temp_location_a->get_x() + temp_hitbox_a->get_width() > temp_location_b->get_x()
                        &&
                        temp_location_a->get_y() < temp_location_b->get_y() + temp_hitbox_b->get_height()
                        && 
                        temp_location_a->get_y() + temp_hitbox_a->get_height() > temp_location_b->get_y()
                    ) {
                        //  Send a message that two entities colided.
                        //  Each entity will get a colision message.
                        //  Ex:  A hit B, B hit A.
                        mgr::messages::add_message(
                            message("entities",
                                    mgr::world::get_name(it_a.first),
                                    mgr::world::get_name(it_b.first),
                                    "colision", "")
                        );
                    }
                } //  End skip self check
            } catch(...) { throw; }
        } //  End it_b loop
    } //  End it_a loop
}

} //  namespace sys

} //  namespace wte
