/*!
 * WTEngine | File:  enabled.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/enabled.hpp"

namespace wte
{

namespace cmp
{

enabled::enabled() : is_enabled(true) {}

enabled::enabled(const bool e) : is_enabled(e) {}

const bool enabled::check(void) const {
    return is_enabled;
}

void enabled::enable(void) {
    is_enabled = true;
}

void enabled::disable(void) {
    is_enabled = false;
}

} //  namespace cmp

} //  namespace wte
