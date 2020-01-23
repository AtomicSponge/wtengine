/*
  WTEngine
  By:  Matthew Evans
  File:  health.hpp

  See LICENSE.txt for copyright information

  Health component
*/

#ifndef WTE_CMP_HEALTH_HPP
#define WTE_CMP_HEALTH_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Health component
/*!
  Store the amount of health an entity has
*/
class health final : public component {
    public:
        inline health(int h) : hp(h) {};

        int hp;
};

} //  namespace cmp

} //  namespace wte

#endif
