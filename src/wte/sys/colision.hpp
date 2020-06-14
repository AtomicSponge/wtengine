/*!
 * \brief WTEngine | File:  colision.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Colision system.
 */

#ifndef WTE_SYS_COLISION_HPP
#define WTE_SYS_COLISION_HPP

#include "system.hpp"

namespace wte
{

namespace sys
{

//!  Colision system
/*!
  Will select components by team and test for a colision
*/
class colision final : public system {
    public:
        inline colision() : system("colision") {};
        inline ~colision() {};

        //!  Allow the colision system to be disabled
        inline void disable(void) override { enabled = false; };

        /*!
         * Overrides virtual run member inherited from system object
         * Selects components by team, then tests each team to see if there is a colision
         * Verifies there is a location and hitbox component when testing
         * Also verifies the entity is enabled and the entity being hit is solid
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t current_time) override {
            const_component_container<cmp::team> team_components = world.get_components<cmp::team>();

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
                    if((it_a != it_b) &&
                       (it_a.second->this_team != it_b.second->this_team) &&
                       world.has_component<cmp::location>(it_a.first) &&
                       world.has_component<cmp::location>(it_b.first) &&
                       world.has_component<cmp::hitbox>(it_a.first) &&
                       world.has_component<cmp::hitbox>(it_b.first) &&
                       world.get_component<cmp::enabled>(it_a.first)->is_enabled &&
                       world.get_component<cmp::enabled>(it_b.first)->is_enabled &&
                       world.get_component<cmp::hitbox>(it_b.first)->solid)
                    {
                        //  Use AABB to test colision
                        if((world.get_component<cmp::location>(it_a.first)->pos_x <
                            world.get_component<cmp::location>(it_b.first)->pos_x +
                            world.get_component<cmp::hitbox>(it_b.first)->width
                            && 
                            world.get_component<cmp::location>(it_a.first)->pos_x +
                            world.get_component<cmp::hitbox>(it_a.first)->width >
                            world.get_component<cmp::location>(it_b.first)->pos_x)
                           &&
                           (world.get_component<cmp::location>(it_a.first)->pos_y <
                            world.get_component<cmp::location>(it_b.first)->pos_y +
                            world.get_component<cmp::hitbox>(it_b.first)->height
                            && 
                            world.get_component<cmp::location>(it_a.first)->pos_y +
                            world.get_component<cmp::hitbox>(it_a.first)->height >
                            world.get_component<cmp::location>(it_b.first)->pos_y))
                        {
                            //  Send a message to the logic system that two entities colided.
                            //  Each entity will get a colision message.
                            //  Ex:  A hit B, B hit A.
                            messages.add_message(message("logic",
                                                        world.get_component<cmp::name>(it_b.first)->name_str,
                                                        world.get_component<cmp::name>(it_a.first)->name_str,
                                                        "colision", ""));
                        }
                    } //  End skip self check
                } //  End it_b loop
            } //  End it_a loop
        }
};

} //  namespace sys

} //  namespace wte

#endif
