/*!
 * WTEngine | File:  animator.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <allegro5/allegro.h>

#include <functional>

#include "wtengine/component.hpp"
#include "wtengine/mgr/entity_manager.hpp"

namespace wte
{

namespace cmp
{

void run(const entity_id& e_id, mgr::entity_manager& world, const int64_t& engine_time) {
    animate(e_id, world, engine_time);
};

void set_drawing(void) {
    al_set_target_bitmap(internal_bitmap);
};

ALLEGRO_BITMAP& get_bitmap(void) const {
    return *internal_bitmap;
};

void set_tint(const ALLEGRO_COLOR& c) {
    tint_color = c;
    tint_set = true;
};

const ALLEGRO_COLOR get_tint(void) const {
    return tint_color;
};

void clear_tint(void) {
    tint_set = false;
};

const bool draw_tinted(void) const {
    return tint_set;
};

} //  namespace cmp

} //  namespace wte
