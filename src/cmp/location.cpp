/*!
 * wtengine | File:  location.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
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

}  //  end namespace wte::cmp
