/*!
 * WTEngine | File:  bounding_box.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/bounding_box.hpp"

namespace wte::cmp {

/*
 *
 */
bounding_box::bounding_box(const float& lx,
             const float& ly,
             const float& rx,
             const float& ry) :
    min_x(lx), min_y(ly), max_x(rx), max_y(ry) {}

/*
 *
 */
void bounding_box::set_min_x(const float& mx) { min_x = mx; }

/*
 *
 */
void bounding_box::set_min_y(const float& my) { min_y = my; }

/*
 *
 */
void bounding_box::set_max_x(const float& mx) { max_x = mx; }

/*
 *
 */
void bounding_box::set_max_y(const float& my) { max_y = my; }

/*
 *
 */
const float bounding_box::get_min_x(void) const { return min_x; }

/*
 *
 */
const float bounding_box::get_min_y(void) const { return min_y; }

/*
 *
 */
const float bounding_box::get_max_x(void) const { return max_x; }

/*
 *
 */
const float bounding_box::get_max_y(void) const { return max_y; }

}  //  end namespace wte::cmp
