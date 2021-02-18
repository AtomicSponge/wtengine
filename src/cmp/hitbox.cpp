/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include "wtengine/component.hpp"

hitbox(float w, float h) : width(w), height(h), solid(true) {}

hitbox(float w, float h, bool s) : width(w), height(h), solid(s) {}

const float get_width(void) const {
    return width;
}

const float get_height(void) const {
    return height;
}

void set_width(const float& w) {
    width = w;
}

void set_height(const float& h) {
    height = h;
}

const bool is_solid(void) const {
    return solid;
}

void make_solid(void) {
    solid = true;
}

void make_fluid(void) {
    solid = false;
}

} //  namespace cmp

} //  namespace wte
