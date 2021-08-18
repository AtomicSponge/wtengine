/*!
 * WTEngine | File:  background.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
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
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const float& x,
    const float& y
) : gfx(bmp, l, [this](const entity_id& e_id) {}), pos_x(x), pos_y(y) {}

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

/*
 *
 */
void background::set_pos_x(const float& x) { pos_x = x; }

/*
 *
 */
void background::set_pos_y(const float& y) { pos_y = y; }

/*
 *
 */
const float background::get_pos_x(void) const { return pos_x; }

/*
 *
 */
const float background::get_pos_y(void) const { return pos_y; }

} //  namespace cmp

} //  namespace wte
