/*
  WTEngine
  By:  Matthew Evans
  File:  health.hpp

  See LICENSE.txt for copyright information

  Health component
*/

#ifndef WTE_ECS_COMPONENT_HEALTH_HPP
#define WTE_ECS_COMPONENT_HEALTH_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Health component
/*!
  Store the amount of health an entity has
*/
class health final : public component {
    public:
        int hp;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
