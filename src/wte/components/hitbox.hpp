/*
  WTEngine
  By:  Matthew Evans
  File:  hitbox.hpp

  See LICENSE.txt for copyright information

  Hitbox component
*/

#ifndef WTE_ECS_COMPONENT_HITBOX_HPP
#define WTE_ECS_COMPONENT_HITBOX_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Hitbox component
/*!
  Store the width and height for an entity's hitbox
*/
class hitbox : public component {
    public:
        float width, height;
        bool solid;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
