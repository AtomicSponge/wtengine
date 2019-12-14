/*
  WTEngine
  By:  Matthew Evans
  File:  visible.hpp

  See LICENSE.txt for copyright information

  Visible component
*/

#ifndef WTE_ECS_COMPONENT_VISIBLE_HPP
#define WTE_ECS_COMPONENT_VISIBLE_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Visible component
/*!
  Toggles if an entity will be drawn by the Renderer
*/
class visible final : public component {
    public:
        bool is_visible;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
