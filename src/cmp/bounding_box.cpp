/*!
 * WTEngine | File:  bounding_box.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/cmp/bounding_box.hpp"

namespace wte::cmp {

/*
 *
 */
bounding_box::bounding_box(
    const float& lx,
    const float& ly,
    const float& rx,
    const float& ry
) : min_x(lx), min_y(ly), max_x(rx), max_y(ry) {}

}  //  end namespace wte::cmp
