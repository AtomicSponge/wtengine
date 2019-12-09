/*
  WTEngine
  By:  Matthew Evans
  File:  team.hpp

  See LICENSE.txt for copyright information

  Health component
*/

#ifndef WTE_ECS_COMPONENT_TEAM_HPP
#define WTE_ECS_COMPONENT_TEAM_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Team component
/*!
  Store the team for an entity
*/
class team : public component {
    public:
        int team;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
