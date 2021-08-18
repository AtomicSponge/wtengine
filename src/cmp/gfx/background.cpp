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
background::background(wte_asset<al_bitmap> bmp, const std::size_t& l, const ALLEGRO_COLOR& c) :
gfx(bmp, l, [this](const entity_id& e_id) {
    set_drawing();
    al_clear_to_color(get_color());
}), color(c) {}

/*
 *
 */
background::background(
    wte_asset<al_bitmap> bmp, const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : gfx(bmp, l, func) {}

/*
 *
 */
background::~background() {}

/*
 *
 */
ALLEGRO_COLOR background::get_color(void) const { return color; }

} //  namespace cmp

} //  namespace wte
