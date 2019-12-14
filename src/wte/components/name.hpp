/*
  WTEngine
  By:  Matthew Evans
  File:  name.hpp

  See LICENSE.txt for copyright information

  Name component
*/

#ifndef WTE_ECS_COMPONENT_NAME_HPP
#define WTE_ECS_COMPONENT_NAME_HPP

#include <string>

#include "component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Name component
/*!
  Store the name of an entity
*/
class name final : public component {
    public:
        std::string name;
};

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
