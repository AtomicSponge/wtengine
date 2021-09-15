/*!
 * WTEngine | File:  team.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_TEAM_HPP
#define WTE_CMP_TEAM_HPP

#include "wtengine/cmp/component.hpp"

namespace wte::cmp {

/*!
 * \class team
 * \brief Store the team for an entity.
 */
class team final : public component {
    public:
        /*!
         * \brief Create a new Team component.
         * 
         * Team can be 0, 1, 2 or 3.
         * 
         * \param t Team value for the entity.
         */
        team(const std::size_t& t);

        ~team() = default;  //!<  Default destructor.

        std::size_t _team;  //!<  Team number.
};

}  //  end namespace wte::cmp

#endif
