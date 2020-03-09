/*
  WTEngine
  By:  Matthew Evans
  File:  damage.hpp

  See LICENSE.txt for copyright information

  Damage component
*/

#ifndef WTE_CMP_DAMAGE_HPP
#define WTE_CMP_DAMAGE_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Damage component
/*!
  Store the amount of damage an entity can do
*/
class damage final : public component {
    public:
        inline damage(int d) : dmg(d) {};

        int dmg;
};

} //  namespace cmp

} //  namespace wte

#endif
