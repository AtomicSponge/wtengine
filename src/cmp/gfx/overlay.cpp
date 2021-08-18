/*!
 * WTEngine | File:  overlay.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/overlay.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
overlay::overlay(
    wte_asset<al_bitmap> bmp,
    wte_asset<al_font> font,
    const float& x,
    const float& y,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : gfx(bmp, l, func), overlay_font(font), pos_x(x), pos_y(y) {}

/*
 *
 */
void overlay::draw_text(
    const std::string& txt,
    const ALLEGRO_COLOR& color,
    const float& x, const float& y,
    const int& f
) {
    al_draw_text(**overlay_font, color, x, y, f, txt.c_str());
}

/*
 *
 */
const float overlay::get_pos_x(void) const { return pos_x; }

/*
 *
 */
const float overlay::get_pos_y(void) const { return pos_y; }

} //  namespace cmp

} //  namespace wte
