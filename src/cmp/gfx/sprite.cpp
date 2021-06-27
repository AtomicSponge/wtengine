/*!
 * WTEngine | File:  sprite.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/sprite.hpp"

namespace wte
{

namespace cmp
{

sprite::sprite(
    const float sw, const float sh,
    const float dox, const float doy,
    const std::size_t rt, const std::size_t l) :
    animator(l, [](const entity_id& e_id) {
            //  Define sprite animation process.
            if(engine_time::check_time() % mgr::entities::get_component<sprite>(e_id)->rate == 0) {
                //  Increment frame.
                mgr::entities::set_component<sprite>(e_id)->current_frame++;
                //  Loop frame.
                if(mgr::entities::get_component<sprite>(e_id)->current_frame > mgr::entities::get_component<sprite>(e_id)->stop_frame) {
                    mgr::entities::set_component<sprite>(e_id)->current_frame = mgr::entities::get_component<sprite>(e_id)->start_frame;
                }
                //  Calculate the X position in the sprite sheet.
                mgr::entities::set_component<sprite>(e_id)->sprite_x = (float)
                    ((int)(mgr::entities::get_component<sprite>(e_id)->current_frame * mgr::entities::get_component<sprite>(e_id)->sprite_width +
                    mgr::entities::get_component<sprite>(e_id)->sheet_width) % mgr::entities::get_component<sprite>(e_id)->sheet_width);
                //  Calculate the Y position in the sprite sheet.
                mgr::entities::set_component<sprite>(e_id)->sprite_y = (float)
                    ((int)((mgr::entities::get_component<sprite>(e_id)->current_frame * mgr::entities::get_component<sprite>(e_id)->sprite_width) /
                    mgr::entities::get_component<sprite>(e_id)->sheet_width) * mgr::entities::get_component<sprite>(e_id)->sprite_height);
            }
        }
    ),
    sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
    sprite_x(0.0f), sprite_y(0.0f), scale_factor_x(1.0f), scale_factor_y(1.0f),
    start_frame(0), stop_frame(0), current_frame(0), rate(rt)
{
    internal_bitmap = NULL;
    if(rate == 0) rate = 1;
}

sprite::~sprite() {
    al_destroy_bitmap(internal_bitmap);
}

void sprite::load_sprite(const std::string& fname) {
    ALLEGRO_FILE* file;
    file = al_fopen(fname.c_str(), "rb");
    if(!file) throw std::runtime_error("Couldn't find sprite file:  " + fname);

    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
    internal_bitmap = al_load_bitmap_f(file, fname.substr(fname.find("."),
                                                fname.length()).c_str());
    al_fclose(file);
    if(!internal_bitmap) throw std::runtime_error("Couldn't load sprite:  " + fname);

    #if WTE_USE_MAGIC_PINK
    al_convert_mask_to_alpha(internal_bitmap, WTE_MAGIC_PINK);
    #endif

    sheet_width = al_get_bitmap_width(internal_bitmap);
    sheet_height = al_get_bitmap_height(internal_bitmap);
}

const bool sprite::add_cycle(
    const std::string& name,
    const std::size_t& start,
    const std::size_t& stop
) {
    auto ret = cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
    return ret.second;
}

const bool sprite::set_cycle(const std::string& name) {
    auto it = cycles.find(name);
    if(it != cycles.end()) {
        start_frame = it->second.first;
        stop_frame = it->second.second;
        return true;
    } else return false;
}

const float sprite::get_sprite_width(void) const {
    return sprite_width;
}

const float sprite::get_sprite_height(void) const {
    return sprite_height;
}

const float sprite::get_draw_offset_x(void) const {
    return draw_offset_x;
}

const float sprite::get_draw_offset_y(void) const {
    return draw_offset_y;
}

const float sprite::get_sprite_x(void) const {
    return sprite_x;
}

const float sprite::get_sprite_y(void) const {
    return sprite_y;
}

void sprite::set_scale_factor_x(const float& sx) {
    scale_factor_x = sx;
}

void sprite::set_scale_factor_y(const float& sy) {
    scale_factor_y = sy;
}

const float sprite::get_scale_factor_x(void) const {
    return scale_factor_x;
}

const float sprite::get_scale_factor_y(void) const {
    return scale_factor_y;
}

} //  namespace cmp

} //  namespace wte
