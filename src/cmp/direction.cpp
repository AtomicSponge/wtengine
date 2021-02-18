/*!
 * WTEngine | File:  direction.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <limits>

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

direction(void) : angle(0.0f), draw_rotated(true) {}

direction(const bool r) : angle(0.0f), draw_rotated(r) {}

direction(const float a) : angle(a), draw_rotated(true) {}

direction(const float a, const bool r) : angle(a), draw_rotated(r) {}

void set_degrees(const float& a) {
    angle = a * M_PI / 180.0f;
}

const float get_degrees(void) const {
    return (angle * 180.0f / M_PI);
}

void set_radians(const float& a) {
    angle = a;
}

const float get_radians(void) const {
    return angle;
}

const bool show_rotated(void) const {
    return draw_rotated;
}

void set_rotated(void) {
    draw_rotated = true;
}

void unset_rotated(void) {
    draw_rotated = false;
}

} //  namespace cmp

} //  namespace wte
