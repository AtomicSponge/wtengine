/*!
 * WTEngine | File:  overlay.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/overlay.hpp"

namespace wte
{

namespace cmp
{

overlay::overlay(
    const int w, const int h,
    const float x, const float y,
    const std::size_t l,
    void func(
        const entity_id&,
        mgr::entity_manager&,
        const int64_t&
    )) : animator(l, func), overlay_w(w), overlay_h(h), pos_x(x), pos_y(y)
{
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(overlay_w, overlay_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    overlay_font = NULL;
}

overlay::~overlay() {
    al_destroy_bitmap(internal_bitmap);
    al_destroy_font(overlay_font);
}

void overlay::reload_overlay_bitmap(void) {
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    ALLEGRO_BITMAP* temp_bmp = al_clone_bitmap(internal_bitmap);
    al_destroy_bitmap(internal_bitmap);
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(overlay_w, overlay_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    al_set_target_bitmap(internal_bitmap);
    al_draw_bitmap(temp_bmp, 0.0f, 0.0f, 0);
    al_destroy_bitmap(temp_bmp);
}

void overlay::draw_text(
    const std::string& txt,
    const ALLEGRO_COLOR& color,
    const float& x, const float& y,
    const int& f
) {
    al_draw_text(overlay_font, color, x, y, f, txt.c_str());
}

const float overlay::get_pos_x(void) const {
    return pos_x;
}

const float overlay::get_pos_y(void) const {
    return pos_y;
}

void overlay::set_font(ALLEGRO_FONT* font) {
    overlay_font = font;
}

} //  namespace cmp

} //  namespace wte
