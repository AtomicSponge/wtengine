/*!
 * WTEngine | File:  sprite.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/sprite.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
sprite::sprite(
    const std::string& bmp,
    const float& sw, const float& sh,
    const float& dox, const float& doy,
    const std::size_t& rt, const std::size_t& l) :
    animator(bmp, l,[this](const entity_id& e_id) {
        //  Define sprite animation process.
        if(engine_time::check_time() % rate == 0) {
            //  Increment frame.
            current_frame++;
            //  Loop frame.
            if(current_frame > stop_frame) {
                current_frame = start_frame;
            }
            //  Calculate the X position in the sprite sheet.
            sprite_x = (float)((int)(current_frame * sprite_width + sheet_width) % sheet_width);
            //  Calculate the Y position in the sprite sheet.
            sprite_y = (float)((int)((current_frame * sprite_width) / sheet_width) * sprite_height);
        }
    }),
    sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
    sprite_x(0.0f), sprite_y(0.0f), scale_factor_x(1.0f), scale_factor_y(1.0f),
    start_frame(0), stop_frame(0), current_frame(0), rate(rt)
{
    if(rate == 0) rate = 1;
    sheet_width = al_get_bitmap_width(*mgr::assets::get<al_bitmap>(get_bitmap()));
    sheet_height = al_get_bitmap_height(*mgr::assets::get<al_bitmap>(get_bitmap()));
}

/*
 *
 */
sprite::~sprite() {}

/*
 * remove
 */
void sprite::load_sprite(const std::string& fname) {}

/*
 *
 */
const bool sprite::add_cycle(
    const std::string& name,
    const std::size_t& start,
    const std::size_t& stop
) {
    auto ret = cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
    return ret.second;
}

/*
 *
 */
const bool sprite::set_cycle(const std::string& name) {
    auto it = cycles.find(name);
    if(it != cycles.end()) {
        start_frame = it->second.first;
        stop_frame = it->second.second;
        return true;
    } else return false;
}

/*
 *
 */
const float sprite::get_sprite_width(void) const { return sprite_width; }

/*
 *
 */
const float sprite::get_sprite_height(void) const { return sprite_height; }

/*
 *
 */
const float sprite::get_draw_offset_x(void) const { return draw_offset_x; }

/*
 *
 */
const float sprite::get_draw_offset_y(void) const { return draw_offset_y; }

/*
 *
 */
const float sprite::get_sprite_x(void) const { return sprite_x; }

/*
 *
 */
const float sprite::get_sprite_y(void) const { return sprite_y; }

/*
 *
 */
void sprite::set_scale_factor_x(const float& sx) { scale_factor_x = sx; }

/*
 *
 */
void sprite::set_scale_factor_y(const float& sy) { scale_factor_y = sy; }

/*
 *
 */
const float sprite::get_scale_factor_x(void) const { return scale_factor_x; }

/*
 *
 */
const float sprite::get_scale_factor_y(void) const { return scale_factor_y; }

} //  namespace cmp

} //  namespace wte
