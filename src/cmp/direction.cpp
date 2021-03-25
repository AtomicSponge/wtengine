/*!
 * WTEngine | File:  direction.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include "wtengine/cmp/direction.hpp"

namespace wte
{

namespace cmp
{

direction::direction(void) : angle(0.0f), draw_rotated(true) {}

direction::direction(const bool r) : angle(0.0f), draw_rotated(r) {}

direction::direction(const float a) : angle(a), draw_rotated(true) {}

direction::direction(const float a, const bool r) : angle(a), draw_rotated(r) {}

void direction::set_degrees(const float& a) {
    angle = a * M_PI / 180.0f;
}

const float direction::get_degrees(void) const {
    return (angle * 180.0f / M_PI);
}

void direction::set_radians(const float& a) {
    angle = a;
}

const float direction::get_radians(void) const {
    return angle;
}

const bool direction::show_rotated(void) const {
    return draw_rotated;
}

void direction::set_rotated(void) {
    draw_rotated = true;
}

void direction::unset_rotated(void) {
    draw_rotated = false;
}

} //  namespace cmp

} //  namespace wte
