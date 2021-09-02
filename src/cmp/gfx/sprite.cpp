/*!
 * WTEngine | File:  sprite.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/sprite.hpp"

namespace wte::cmp {

/*
 *
 */
sprite::sprite(
    wte_asset<al_bitmap> bmp,
    const float& sw, const float& sh,
    const float& dox, const float& doy,
    const std::size_t& rt, const std::size_t& l) :
    gfx(bmp, l,[this](const entity_id& e_id) {
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
    sprite_x(0.0f), sprite_y(0.0f),
    start_frame(0), stop_frame(0), current_frame(0), rate(rt)
{
    if(rate == 0) rate = 1;
    try {
        sheet_width = internal_bitmap->get_width();
        sheet_height = internal_bitmap->get_height();
    } catch(...) {
        throw wte_exception("Error creating sprite.  Cannot access asset.", "sprite", engine_time::check_time());
    }
}

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

}  //  end namespace wte::cmp
