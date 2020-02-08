/*
  WTEngine
  By:  Matthew Evans
  File:  colision.hpp

  See LICENSE.txt for copyright information

  Colision system
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
class colision : public system {
    public:
        inline colision() : system("colision") {};
        inline ~colision() {};

        //!  Allow the colision system to be disabled
        inline void disable(void) { enabled = false; };

        //!  Run the colision system
        inline void run(mgr::entity_manager&, mgr::message_manager&, int64_t) final;
};

/*!
  Overrides virtual run member inherited from system object
  Selects components by team, then tests each team to see if there is a colision
  Verifies there is a location and hitbox component when testing
  Also verifies the entity is enabled and the entity being hit is solid
*/
inline void colision::run(mgr::entity_manager& world,
                          mgr::message_manager& messages,
                          int64_t current_time) {
    component_container team_components;

    team_components = world.get_components<cmp::team>();

    for(component_iterator it_a = team_components.begin(); it_a != team_components.end(); it_a++) {
        for(component_iterator it_b = team_components.begin(); it_b != team_components.end(); it_b++) {
            //  Only test different teams, if the entity has a location and a hitbox component, and it is enabled
            if((dynamic_cast<cmp::team*>(it_a->second.get())->this_team != dynamic_cast<cmp::team*>(it_b->second.get())->this_team)
               &&
               (world.has_component<cmp::location>(it_a->first) && world.has_component<cmp::location>(it_b->first))
               &&
               (world.has_component<cmp::hitbox>(it_a->first) && world.has_component<cmp::hitbox>(it_b->first))
               &&
               (world.get_component<cmp::enabled>(it_a->first)->is_enabled == true && world.get_component<cmp::enabled>(it_b->first)->is_enabled == true)
               &&
               world.get_component<cmp::hitbox>(it_b->first)->solid == true)
            {
                //  Use AABB to test colision
                if((world.get_component<cmp::location>(it_a->first)->pos_x < world.get_component<cmp::location>(it_b->first)->pos_x + world.get_component<cmp::hitbox>(it_b->first)->width && 
                    world.get_component<cmp::location>(it_a->first)->pos_x + world.get_component<cmp::hitbox>(it_a->first)->width > world.get_component<cmp::location>(it_b->first)->pos_x)
                   &&
                   (world.get_component<cmp::location>(it_a->first)->pos_y < world.get_component<cmp::location>(it_b->first)->pos_y + world.get_component<cmp::hitbox>(it_b->first)->height && 
                    world.get_component<cmp::location>(it_a->first)->pos_y + world.get_component<cmp::hitbox>(it_a->first)->height > world.get_component<cmp::location>(it_b->first)->pos_y))
                {
                    //  Send a message to the logic system that two entities colided
                    messages.add_message(msg::message("logic",
                                                      world.get_component<cmp::name>(it_b->first)->name_str,
                                                      world.get_component<cmp::name>(it_a->first)->name_str,
                                                      "colision", ""));
                }
            } //  End skip self check
        } //  End it_b loop
    } //  End it_a loop
}

} //  namespace sys

} //  namespace wte

#endif
