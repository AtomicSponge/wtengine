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

/*!
 * \class Team component
 * Store the team for an entity
 */
class team final : public component {
    public:
        /*!
         * \brief Team constructor.
         * Team can be 0, 1, 2 or 3.
         * \param t Team value for the entity.
         * \return void
         */
        inline team(std::size_t t) : this_team(t) {
            if(this_team > 3) this_team = 3;
        };

        /*!
         *
         */
        inline const std::size_t get_team(void) const {
            return this_team;
        };

        /*!
         *
         */
        inline void set_team(const std::size_t& t) {
            this_team = t;
        };

    private:
        std::size_t this_team;
};

} //  namespace cmp

} //  namespace wte

#endif
