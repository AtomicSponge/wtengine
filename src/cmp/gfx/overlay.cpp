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
    std::shared_ptr<wte_asset> bmp,
    const float& x, const float& y,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : animator(bmp, l, func), pos_x(x), pos_y(y)
{ overlay_font = NULL; }

/*
 *
 */
overlay::~overlay() {
    al_destroy_font(overlay_font);
}

/*
 *
 */
void overlay::draw_text(
    const std::string& txt,
    const ALLEGRO_COLOR& color,
    const float& x, const float& y,
    const int& f
) {
    al_draw_text(overlay_font, color, x, y, f, txt.c_str());
}

/*
 *
 */
const float overlay::get_pos_x(void) const { return pos_x; }

/*
 *
 */
const float overlay::get_pos_y(void) const { return pos_y; }

/*
 *
 */
void overlay::set_font(ALLEGRO_FONT* font) { overlay_font = font; }

} //  namespace cmp

} //  namespace wte
