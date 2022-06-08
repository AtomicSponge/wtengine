/*!
 * wtengine | File:  overlay.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/cmp/overlay.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
overlay::overlay(
    wte_asset<al_bitmap> bmp,
    wte_asset<al_font> font,
    const std::size_t& l,
    const float& x,
    const float& y,
    const std::function<void(const entity_id&)>& func
) : gfx(bmp, l, func), pos_x(x), pos_y(y), overlay_font(font) {}

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

}  //  end namespace wte::cmp
