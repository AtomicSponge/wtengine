/*!
 * WTEngine | File:  hitbox.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/hitbox.hpp"

namespace wte::cmp {

/*
 *
 */
hitbox::hitbox(
    const float& w,
    const float& h
) : width(w), height(h), solid(true) {}

/*
 *
 */
hitbox::hitbox(
    const float& w,
    const float& h,
    const bool& s
) : width(w), height(h), solid(s) {}

}  //  end namespace wte::cmp
