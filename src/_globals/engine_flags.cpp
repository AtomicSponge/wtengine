/*!
 * WTEngine | File:  engine_flags.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <cassert>

#include "wtengine/_globals/engine_flags.hpp"

namespace wte
{

static void engine_flags::set(const std::size_t& i) {
    assert(i < MAX_SYSTEM_FLAGS);
    flags[i] = true;
}

static void engine_flags::unset(const std::size_t& i) {
    assert(i < MAX_SYSTEM_FLAGS);
    flags[i] = false;
}

static const bool engine_flags::is_set(const std::size_t& i) {
    assert(i < MAX_SYSTEM_FLAGS);
    return flags[i];
}

static void engine_flags::unset_all(void) {
    for(std::size_t i = 0; i < MAX_SYSTEM_FLAGS; i++)
        flags[i] = false;
}

} //  end namespace wte

#endif
