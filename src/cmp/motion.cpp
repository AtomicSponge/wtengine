/*!
 * WTEngine | File:  motion.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/motion.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
motion::motion(void) : direction(0), x_vel(0.0f), y_vel(0.0f) {}

/*
 *
 */
motion::motion(
    const float& d,
    const float& v
) : direction(d), x_vel(v), y_vel(0.0f) {}

/*
 *
 */
motion::motion(
    const float& d,
    const float& xv,
    const float& yv
) : direction(d), x_vel(xv), y_vel(yv) {}

/*
 *
 */
void motion::set_direction(const float& d) { direction = d; }

/*
 *
 */
const float motion::get_direction(void) const { return direction; }

/*
 *
 */
void motion::set_x_vel(const float& xv) { x_vel = xv; }

/*
 *
 */
void motion::set_y_vel(const float& yv) { y_vel = yv; }

/*
 *
 */
const float motion::get_x_vel(void) const { return x_vel; }

/*
 *
 */
const float motion::get_y_vel(void) const { return y_vel; }

/*
 *
 */
void motion::set_velocity(const float& v) {
    x_vel = v;
    y_vel = v;
}

/*
 *
 */
const float motion::get_velocity(void) const { return x_vel; }

} //  namespace cmp

} //  namespace wte
