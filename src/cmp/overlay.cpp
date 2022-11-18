/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/cmp/overlay.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
overlay::overlay(
    wte_asset<ALLEGRO_BITMAP> bmp,
    wte_asset<ALLEGRO_FONT> font,
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
    al_draw_text(overlay_font.get(), color, x, y, f, txt.c_str());
}

}  //  end namespace wte::cmp
