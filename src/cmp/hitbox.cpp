/*!
 * WTEngine | File:  hitbox.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/hitbox.hpp"

namespace wte::cmp {

/*
 *
 */
hitbox::hitbox(const float& w, const float& h) : width(w), height(h), solid(true) {}

/*
 *
 */
hitbox::hitbox(const float& w, const float& h, const bool& s) : width(w), height(h), solid(s) {}

/*
 *
 */
const float hitbox::get_width(void) const { return width; }

/*
 *
 */
const float hitbox::get_height(void) const { return height; }

/*
 *
 */
void hitbox::set_width(const float& w) { width = w; }

/*
 *
 */
void hitbox::set_height(const float& h) { height = h; }

/*
 *
 */
const bool hitbox::is_solid(void) const { return solid; }

/*
 *
 */
void hitbox::make_solid(void) { solid = true; }

/*
 *
 */
void hitbox::make_fluid(void) { solid = false; }

}  //  end namespace wte::cmp
