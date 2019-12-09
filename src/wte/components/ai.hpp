/*
  WTEngine
  By:  Matthew Evans
  File:  ai.hpp

  See LICENSE.txt for copyright information

  AI component - tag to be processed by logic system
*/

#ifndef WTE_ECS_COMPONENT_AI_HPP
#define WTE_ECS_COMPONENT_AI_HPP

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! AI component
/*!
  Tag components to be processed by the Logic system
  Has no members
*/
class ai : public component {
    //  No members
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
