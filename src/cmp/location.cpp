/*!
 * WTEngine | File:  location.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/location.hpp"

namespace wte::cmp {

/*
 *
 */
location::location(
    const float& x,
    const float& y
) : pos_x(x), pos_y(y) {};

/*
 *
 */
const float location::get_x(void) const { return pos_x; }

/*
 *
 */
const float location::get_y(void) const { return pos_y; }

/*
 *
 */
void location::set_x(const float& x) { pos_x = x; }

/*
 *
 */
void location::set_y(const float& y) { pos_y = y; }

/*
 *
 */
void location::adjust_x(const float& x) { pos_x += x; }

/*
 *
 */
void location::adjust_y(const float& y) { pos_y += y; }

}  //  end namespace wte::cmp
