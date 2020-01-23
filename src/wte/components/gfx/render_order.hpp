/*
  WTEngine
  By:  Matthew Evans
  File:  render_order.hpp

  See LICENSE.txt for copyright information

  Render order component
*/

#ifndef WTE_CMP_RENDER_ORDER_HPP
#define WTE_CMP_RENDER_ORDER_HPP

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Render Order component
/*!
  Used to sort entities in the Renderer
*/
class render_order : public component {
    public:
        inline render_order(int o) : order(o) {};

        int order;
};

} //  namespace cmp

} //  namespace wte

#endif
