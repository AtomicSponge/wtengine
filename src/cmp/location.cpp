/*!
 * WTEngine | File:  location.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

location(float x, float y) : pos_x(x), pos_y(y) {};

const float get_x(void) const {
    return pos_x;
}

const float get_y(void) const {
    return pos_y;
}

void set_x(const float& x) {
    pos_x = x;
}

void set_y(const float& y) {
    pos_y = y;
}

void adjust_x(const float& x) {
    pos_x += x;
}

void adjust_y(const float& y) {
    pos_y += y;
}

} //  namespace cmp

} //  namespace wte
