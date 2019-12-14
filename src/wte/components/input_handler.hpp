/*
  WTEngine
  By:  Matthew Evans
  File:  input_handler.hpp

  See LICENSE.txt for copyright information

  Input handler component - tag to be processed by input system
*/

#ifndef WTE_ECS_COMPONENT_INPUT_HANDLER_HPP
#define WTE_ECS_COMPONENT_INPUT_HANDLER_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Input Handler component
/*!
  Tag an entity to be processed by the Input system
*/
class input_handler final : public component {
    //  No members
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
