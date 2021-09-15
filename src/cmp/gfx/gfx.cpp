/*!
 * WTEngine | File:  gfx.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
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

}  //  end namespace wte::cmp
