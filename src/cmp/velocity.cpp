/*!
 * WTEngine | File:  velocity.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

velocity::velocity(void) : x_vel(0.0f), y_vel(0.0f) {}

velocity::velocity(const float v) : x_vel(v), y_vel(v) {}

velocity::velocity(const float xv, const float yv) : x_vel(xv), y_vel(yv) {}

void velocity::set_x_vel(const float& xv) {
    x_vel = xv;
}

void velocity::set_y_vel(const float& yv) {
    y_vel = yv;
}

void velocity::set_velocity(const float& v) {
    x_vel = v;
    y_vel = v;
}

const float velocity::get_x_vel(void) const {
    return x_vel;
}

const float velocity::get_y_vel(void) const {
    return y_vel;
}

} //  namespace cmp

} //  namespace wte

#endif
