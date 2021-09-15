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
team::team(const std::size_t& t) : _team(t) {
    if(_team > 3) _team = 3;
}

}  //  end namespace wte::cmp
