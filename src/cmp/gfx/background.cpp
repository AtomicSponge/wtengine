/*!
 * WTEngine | File:  background.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/background.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
background::background(
    const int w, const int h,
    const std::size_t l, ALLEGRO_COLOR c) :
    animator(l, [this](const entity_id& e_id) {
        set_drawing();
        al_clear_to_color(get_color());
    }), background_w(w), background_h(h), color(c)
{
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(background_w, background_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
};

/*
 *
 */
background::background(
    const int w, const int h, const std::size_t l,
    std::function<void(const entity_id&)> func
) : animator(l, func), background_w(w), background_h(h)
{
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(background_w, background_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
};

/*
 *
 */
background::~background() { al_destroy_bitmap(internal_bitmap); };

/*
 *
 */
void background::reload_background_bitmap(void) {
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    ALLEGRO_BITMAP* temp_bmp = al_clone_bitmap(internal_bitmap);
    al_destroy_bitmap(internal_bitmap);
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(background_w, background_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    al_set_target_bitmap(internal_bitmap);
    al_draw_bitmap(temp_bmp, 0.0f, 0.0f, 0);
    al_destroy_bitmap(temp_bmp);
};

/*
 *
 */
ALLEGRO_COLOR background::get_color(void) const { return color; };

} //  namespace cmp

} //  namespace wte
