/*
  WTEngine
  By:  Matthew Evans
  File:  component.hpp

  See LICENSE.txt for copyright information

  Component interface class
  Extend this to create a new component
*/

#ifndef WTE_CMP_COMPONENT_HPP
#define WTE_CMP_COMPONENT_HPP

#include <memory>

namespace wte
{

namespace cmp
{

//! Component interface class
/*!
  Extend this to create a component
*/
class component {
    public:
        inline virtual ~component() {};

        component(const component&) = delete;
        void operator=(component const&) = delete;

    protected:
        inline component() {};
};

//! Component shared pointer
typedef std::shared_ptr<cmp::component> component_sptr;

} //  namespace cmp

} //  namespace wte

#endif
