/*!
 * WTEngine | File:  background.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <map>

#include <allegro5/allegro.h>

#include "wtengine/animator.hpp"
#include "wtengine/bitmap_map.hpp"

namespace wte
{

namespace cmp
{

background::background(const int w, const int h,
                    const std::size_t l, ALLEGRO_COLOR c) :
    animator(l, [](const entity_id& e_id, mgr::entity_manager& world, const int64_t& engine_time)
    {
        world.set_component<cmp::background>(e_id)->set_drawing();
        al_clear_to_color(world.get_component<cmp::background>(e_id)->get_color());
    }), background_w(w), background_h(h), color(c)
{
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(background_w, background_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
};

background::background(const int w, const int h, const std::size_t l,
                    void func(const entity_id&, mgr::entity_manager&, const int64_t&)) :
    animator(l, func), background_w(w), background_h(h)
{
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
    internal_bitmap = al_create_bitmap(background_w, background_h);
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
};

background::~background() {
    al_destroy_bitmap(internal_bitmap);
};

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

ALLEGRO_COLOR background::get_color(void) const {
    return color;
};

} //  namespace cmp

} //  namespace wte
