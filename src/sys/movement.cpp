/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/sys/movement.hpp"

namespace wte::sys {

/*
 *
 */
movement::movement() : system("movement") {}

/*
 *
 */
void movement::run(void) {
    //  Find the entities with a motion component.
    const_component_container<cmp::motion> vel_components
        = mgr::world::get_components<cmp::motion>();
    for(auto& it: vel_components) {
        cmp::const_comp_ptr<cmp::location> temp_get =
            mgr::world::get_component<cmp::location>(it.first);
        cmp::comp_ptr<cmp::location> temp_set =
            mgr::world::set_component<cmp::location>(it.first);

        temp_set->pos_x += (it.second->x_vel * std::cos(it.second->direction));
        temp_set->pos_y += (it.second->y_vel * std::sin(it.second->direction));
    }

    //  Now check all bounding boxes.
    const_component_container<cmp::bounding_box> bbox_components =
        mgr::world::get_components<cmp::bounding_box>();
    for(auto& it: bbox_components) {
        cmp::const_comp_ptr<cmp::location> temp_get =
            mgr::world::get_component<cmp::location>(it.first);
        cmp::comp_ptr<cmp::location> temp_set =
            mgr::world::set_component<cmp::location>(it.first);

        if(temp_get->pos_x < it.second->min_x) temp_set->pos_x = it.second->min_x;
        else if(temp_get->pos_x > it.second->max_x) temp_set->pos_x = it.second->max_x;

        if(temp_get->pos_y < it.second->min_y) temp_set->pos_y = it.second->min_y;
        else if(temp_get->pos_y > it.second->max_y) temp_set->pos_y = it.second->max_y;
    }
}

}  //  end namespace wte::sys
