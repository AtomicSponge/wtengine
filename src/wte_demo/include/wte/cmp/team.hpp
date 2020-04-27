/*!
 * \brief WTEngine | File:  team.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Team component.
 */

#ifndef WTE_CMP_TEAM_HPP
#define WTE_CMP_TEAM_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Team component
/*!
 * Store the team for an entity
 */
class team final : public component {
    public:
        inline team(int t) : this_team(t) {};

        int this_team;
};

} //  namespace cmp

} //  namespace wte

#endif
