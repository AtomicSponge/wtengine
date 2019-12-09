/*
  WTEngine
  By:  Matthew Evans
  File:  velocity.hpp

  See LICENSE.txt for copyright information

  Velocity component
*/

#ifndef WTE_ECS_COMPONENT_VELOCITY_HPP
#define WTE_ECS_COMPONENT_VELOCITY_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Velocity component
/*!
  Store the movement velocity of an entity
*/
class velocity : public component {
    public:
        float speed;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
