/*!
 * wtengine | File:  gfx.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/cmp/gfx.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
gfx::gfx(
    wte_asset<ALLEGRO_BITMAP> bmp,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : layer(l), visible(true), rotated(false), direction(0.0f),
scale_factor_x(1.0f), scale_factor_y(1.0f),
_bitmap(bmp), tinted(false), animate(func) {}

/*
 *
 */
void gfx::set_drawing(void) { al_set_target_bitmap(_bitmap.get()); }

/*
 *
 */
void gfx::set_tint(const ALLEGRO_COLOR& c) {
    tint_color = c;
    tinted = true;
}

/*
 *
 */
const ALLEGRO_COLOR gfx::get_tint(void) const { return tint_color; }

/*
 *
 */
void gfx::clear_tint(void) { tinted = false; }

}  //  end namespace wte::cmp
