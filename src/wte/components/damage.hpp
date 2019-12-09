/*
  WTEngine
  By:  Matthew Evans
  File:  damage.hpp

  See LICENSE.txt for copyright information

  Damage component
*/

#ifndef WTE_ECS_COMPONENT_DAMAGE_HPP
#define WTE_ECS_COMPONENT_DAMAGE_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Damage component
/*!
  Store the amount of damage an entity can do
*/
class damage : public component {
    public:
        int dmg;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
