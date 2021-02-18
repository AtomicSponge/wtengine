/*!
 * WTEngine | File:  visible.hpp
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

visible() : is_visible(true) {}

visible(const bool v) : is_visible(v) {}

const bool check(void) const {
    return is_visible;
}

void show(void) {
    is_visible = true;
}

void hide(void) {
    is_visible = false;
}

} //  namespace cmp

} //  namespace wte

#endif
