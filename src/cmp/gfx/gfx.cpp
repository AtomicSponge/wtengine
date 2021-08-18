/*!
 * WTEngine | File:  gfx.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/gfx.hpp"

namespace wte {

namespace cmp {

/*
 *
 */
gfx::gfx(
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : internal_bitmap(bmp), layer(l), tint_set(false), visible(true), direction(0.0f), rotated(false), animate(func) {}

/*
 *
 */
void gfx::run(const entity_id& e_id) { try { animate(e_id); } catch(...) { throw; } }

/*
 *
 */
void gfx::set_drawing(void) { al_set_target_bitmap(**internal_bitmap); }

/*
 *
 */
ALLEGRO_BITMAP* gfx::get_bitmap(void) const { return **internal_bitmap; }

/*
 *
 */
void gfx::set_tint(const ALLEGRO_COLOR& c) {
    tint_color = c;
    tint_set = true;
}

/*
 *
 */
const ALLEGRO_COLOR gfx::get_tint(void) const { return tint_color; }

/*
 *
 */
void gfx::clear_tint(void) { tint_set = false; }

/*
 *
 */
const bool gfx::draw_tinted(void) const { return tint_set; }

/*
 *
 */
void gfx::show(void) { visible = true; }

/*
 *
 */
void gfx::hide(void) { visible = false; }

/*
 *
 */
const bool gfx::is_visible(void) const { return visible; }

/*
 *
 */
void gfx::is_roated(void) { rotated = true; }

/*
 *
 */
void gfx::not_roated(void) { rotated = false; }

/*
 *
 */
const bool gfx::draw_rotated(void) const { return rotated; }

/*
 *
 */
void gfx::set_direction(const float& d) { direction = d; }

/*
 *
 */
const float gfx::get_direction(void) const { return direction; }

} //  namespace cmp

} //  namespace wte
