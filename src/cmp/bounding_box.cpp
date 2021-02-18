/*!
 * WTEngine | File:  bounding_box.hpp
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

bounding_box(const float lx,
             const float ly,
             const float rx,
             const float ry) :
    min_x(lx), min_y(ly), max_x(rx), max_y(ry) {}

void set_min_x(const float& mx) {
    min_x = mx;
}

void set_min_y(const float& my) {
    min_y = my;
}

void set_max_x(const float& mx) {
    max_x = mx;
}

void set_max_y(const float& my) {
    max_y = my;
}

const float get_min_x(void) const {
    return min_x;
}

const float get_min_y(void) const {
    return min_y;
}

const float get_max_x(void) const {
    return max_x;
}

const float get_max_y(void) const {
    return max_y;
}

} //  namespace cmp

} //  namespace wte

#endif
