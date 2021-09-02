/*!
 * WTEngine | File:  gfx.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/gfx.hpp"

namespace wte::cmp {

/*
 *
 */
gfx::gfx(
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : internal_bitmap(bmp), layer(l), tint_set(false), visible(true),
direction(0.0f), rotated(false), scale_factor_x(1.0f), scale_factor_y(1.0f),
animate(func) {}

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
void gfx::set_rotation(const bool& r) { rotated = r; }

/*
 *
 */
const bool gfx::is_rotated(void) const { return rotated; }

/*
 *
 */
void gfx::set_direction(const float& d) { direction = d; }

/*
 *
 */
const float gfx::get_direction(void) const { return direction; }

/*
 *
 */
void gfx::set_scale_factor_x(const float& sx) { scale_factor_x = sx; }

/*
 *
 */
void gfx::set_scale_factor_y(const float& sy) { scale_factor_y = sy; }

/*
 *
 */
const float gfx::get_scale_factor_x(void) const { return scale_factor_x; }

/*
 *
 */
const float gfx::get_scale_factor_y(void) const { return scale_factor_y; }

}  //  end namespace wte::cmp
