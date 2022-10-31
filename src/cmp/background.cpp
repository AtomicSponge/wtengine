/*!
 * wtengine | File:  background.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/cmp/background.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
background::background(
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const float& x,
    const float& y
) : gfx(bmp, l, [](const entity_id& e_id) {}), pos_x(x), pos_y(y) {}

/*
 *
 */
background::background(
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const float& x,
    const float& y,
    const std::function<void(const entity_id&)>& func
) : gfx(bmp, l, func), pos_x(x), pos_y(y) {}

}  //  end namespace wte::cmp
