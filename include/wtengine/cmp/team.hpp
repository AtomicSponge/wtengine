/*!
 * WTEngine | File:  team.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_TEAM_HPP
#define WTE_CMP_TEAM_HPP

#include "wtengine/cmp/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class team
 * \brief Store the team for an entity.
 */
class team final : public component {
    public:
        /*!
         * \brief Team constructor.
         * 
         * Team can be 0, 1, 2 or 3.
         * 
         * \param t Team value for the entity.
         */
        team(std::size_t t);

        /*!
         * \brief Get team.
         * 
         * \return Team the entity is on.
         */
        const std::size_t get_team(void) const;

        /*!
         * \brief Set team
         * 
         * \param t New team for the entity.
         */
        void set_team(const std::size_t& t);

    private:
        std::size_t this_team;
};

} //  namespace cmp

} //  namespace wte

#endif
