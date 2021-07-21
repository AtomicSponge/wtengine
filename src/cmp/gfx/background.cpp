/*!
 * WTEngine | File:  background.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
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
background::background(const std::shared_ptr<wte_asset> bmp, const std::size_t& l, const ALLEGRO_COLOR& c) :
animator(bmp, l, [this](const entity_id& e_id) {
    set_drawing();
    al_clear_to_color(get_color());
}), color(c) {}

/*
 *
 */
background::background(
    const std::shared_ptr<wte_asset>& bmp, const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : animator(bmp, l, func) {}

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
