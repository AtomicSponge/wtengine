/*
  WTEngine
  By:  Matthew Evans
  File:  colision.hpp

  See LICENSE.txt for copyright information

  Animation system
  Updates all sprite components
*/

#ifndef WTE_ECS_SYSTEM_ANIMATE_HPP
#define WTE_ECS_SYSTEM_ANIMATE_HPP

#include "system.hpp"

namespace wte
{

namespace ecs
{

namespace sys
{

//! Animate system
/*!
  WIP
*/
class animate : public system {
    public:
        inline animate() { name = "animate"; };
        inline ~animate() {};
        
        inline void run(entity_manager&, msg::message_queue&, int64_t);
};

//! Animate system run member
/*!
  WIP
*/
inline void animate::run(entity_manager& world, msg::message_queue& messages, int64_t current_time) {
    component_container sprite_components;

    sprite_components = world.get_components<cmp::sprite>();
}

} //  namespace sys

} //  namespace ecs

} //  namespace wte

#endif
