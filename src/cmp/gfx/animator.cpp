/*!
 * WTEngine | File:  animator.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
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
    wte_asset<al_bitmap> bmp,
    const std::size_t& l,
    const std::function<void(const entity_id&)>& func
) : internal_bitmap(bmp), layer(l), tint_set(false), visible(true), direction(0.0f), rotated(false), animate(func) {}

/*
 *
 */
void animator::run(const entity_id& e_id) { try { animate(e_id); } catch(...) { throw; } }

/*
 *
 */
void animator::set_drawing(void) { al_set_target_bitmap(**internal_bitmap); }

/*
 *
 */
ALLEGRO_BITMAP* animator::get_bitmap(void) const { return **internal_bitmap; }

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

/*
 *
 */
void animator::show(void) { visible = true; }

/*
 *
 */
void animator::hide(void) { visible = false; }

/*
 *
 */
const bool animator::is_visible(void) const { return visible; }

/*
 *
 */
void animator::is_roated(void) { rotated = true; }

/*
 *
 */
void animator::not_roated(void) { rotated = false; }

/*
 *
 */
const bool animator::draw_rotated(void) const { return rotated; }

/*
 *
 */
void animator::set_direction(const float& d) { direction = d; }

/*
 *
 */
const float animator::get_direction(void) const { return direction; }

} //  namespace cmp

} //  namespace wte
