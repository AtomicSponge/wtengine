/*!
 * WTEngine | File:  enabled.hpp
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
