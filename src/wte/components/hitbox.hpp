/*
  WTEngine
  By:  Matthew Evans
  File:  hitbox.hpp

  See LICENSE.txt for copyright information

  Hitbox component
*/

#ifndef WTE_CMP_HITBOX_HPP
#define WTE_CMP_HITBOX_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Hitbox component
/*!
  Store the width and height for an entity's hitbox
*/
class hitbox final : public component {
    public:
        inline hitbox(float w, float h) : width(w), height(h), solid(true) {};
        inline hitbox(float w, float h, bool s) : width(w), height(h), solid(s) {};

        float width, height;
        bool solid;
};

} //  namespace cmp

} //  namespace wte

#endif
