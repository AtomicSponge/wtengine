/*!
 * WTEngine | File:  collision.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/collision.hpp"

namespace wte::sys {

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
                    it_a.second->_team != it_b.second->_team &&
                    temp_hitbox_a->solid && temp_hitbox_b->solid
                ) {
                    //  Use AABB to test colision
                    if(
                        temp_location_a->pos_x < temp_location_b->pos_x + temp_hitbox_b->width
                        && 
                        temp_location_a->pos_x + temp_hitbox_a->width > temp_location_b->pos_x
                        &&
                        temp_location_a->pos_y < temp_location_b->pos_y + temp_hitbox_b->height
                        && 
                        temp_location_a->pos_y + temp_hitbox_a->height > temp_location_b->pos_y
                    ) {
                        //  Send a message that two entities colided.
                        //  Each entity will get a colision message.
                        //  Ex:  A hit B, B hit A.
                        mgr::messages::add(
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

}  //  end namespace wte::sys
