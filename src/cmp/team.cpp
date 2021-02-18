/*!
 * WTEngine | File:  team.hpp
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

team(std::size_t t) : this_team(t) {
    if(this_team > 3) this_team = 3;
}

const std::size_t get_team(void) const {
    return this_team;
}

void set_team(const std::size_t& t) {
    this_team = t;
}

} //  namespace cmp

} //  namespace wte
