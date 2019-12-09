/*
  WTEngine
  By:  Matthew Evans
  File:  render_order.hpp

  See LICENSE.txt for copyright information

  Render order component
*/

#ifndef WTE_ECS_COMPONENT_RENDER_ORDER_HPP
#define WTE_ECS_COMPONENT_RENDER_ORDER_HPP

#include "../component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Render Order component
/*!
  Used to sort entities in the Renderer
*/
class render_order : public component {
    public:
        int order;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
