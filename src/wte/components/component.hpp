/*
  WTEngine
  By:  Matthew Evans
  File:  component.hpp

  See LICENSE.txt for copyright information

  Component interface class
  Extend this to create a new component
*/

#ifndef WTE_ECS_COMPONENT_HPP
#define WTE_ECS_COMPONENT_HPP

#include <memory>

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Component interface class
/*!
  Extend this to create a component
*/
class component {
    protected:
        virtual void interface(void) {};
};

//! Component shared pointer
typedef std::shared_ptr<component> component_sptr;

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
