/*!
 * WTEngine | File:  sprite.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/sprite.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
sprite::sprite(
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const float& sw, const float& sh,
    const float& dox, const float& doy,
    const std::size_t& rt) :
    gfx(bmp, l,[this](const entity_id& e_id) {
        //  Define sprite animation process.
        if(engine_time::check() % rate == 0) {
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
    sheet_width = _bitmap->get_width();
    sheet_height = _bitmap->get_height();
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

}  //  end namespace wte::cmp
