/*!
 * WTEngine | File:  team.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/team.hpp"

namespace wte::cmp {

/*
 *
 */
team::team(const std::size_t& t) : this_team(t) {
    if(this_team > 3) this_team = 3;
}

/*
 *
 */
const std::size_t team::get_team(void) const { return this_team; }

/*
 *
 */
void team::set_team(const std::size_t& t) { this_team = t; }

}  //  end namespace wte::cmp
