/*!
 * WTEngine | File:  animator.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/gfx/animator.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
animator::animator(
    const std::string& bmp,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : internal_bitmap(bmp), layer(l), tint_set(false), animate(func) {}

/*
 *
 */
void animator::run(const entity_id& e_id) { animate(e_id); }

/*
 *
 */
void animator::set_drawing(void) { al_set_target_bitmap(mgr::assets::get(internal_bitmap)); }

/*
 *
 */
const std::string animator::get_bitmap(void) const { return internal_bitmap; }

/*
 *
 */
void animator::set_tint(const ALLEGRO_COLOR& c) {
    tint_color = c;
    tint_set = true;
}

/*
 *
 */
const ALLEGRO_COLOR animator::get_tint(void) const { return tint_color; }

/*
 *
 */
void animator::clear_tint(void) { tint_set = false; }

/*
 *
 */
const bool animator::draw_tinted(void) const { return tint_set; }

} //  namespace cmp

} //  namespace wte
