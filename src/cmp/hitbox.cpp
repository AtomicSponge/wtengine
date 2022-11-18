/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/cmp/hitbox.hpp"

namespace wte::cmp {

/*
 *
 */
hitbox::hitbox(
    const float& w,
    const float& h,
    const std::size_t& t
) : width(w), height(h), team(t), solid(true) {}

/*
 *
 */
hitbox::hitbox(
    const float& w,
    const float& h,
    const std::size_t& t,
    const bool& s
) : width(w), height(h), team(t), solid(s) {}

}  //  end namespace wte::cmp
