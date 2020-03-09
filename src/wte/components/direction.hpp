/*
  WTEngine
  By:  Matthew Evans
  File:  direction.hpp

  See LICENSE.txt for copyright information

  Direction component
*/

#ifndef WTE_CMP_DIRECTION_HPP
#define WTE_CMP_DIRECTION_HPP

#include "component.hpp"

namespace wte
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

} //  namespace wte

#endif
