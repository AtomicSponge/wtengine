/*
  WTEngine
  By:  Matthew Evans
  File:  location.hpp

  See LICENSE.txt for copyright information

  Location component
*/

#ifndef WTE_ECS_COMPONENT_LOCATION_HPP
#define WTE_ECS_COMPONENT_LOCATION_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Location component
/*!
  Store the x/y location of an entity
*/
class location : public component {
    public:
        float pos_x, pos_y;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
