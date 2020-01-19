/*
  WTEngine
  By:  Matthew Evans
  File:  direction.hpp

  See LICENSE.txt for copyright information

  Direction component
*/

#ifndef WTE_ECS_COMPONENT_DIRECTION_HPP
#define WTE_ECS_COMPONENT_DIRECTION_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Direction component
/*!
  Store the direction of travel for an entity
*/
class direction final : public component {
    public:
        inline direction(float a) : angle(a) {};

        float angle;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
