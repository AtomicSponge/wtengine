/*!
 * \brief WTEngine | File:  health.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Health component.
 */

#ifndef WTE_CMP_HEALTH_HPP
#define WTE_CMP_HEALTH_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Health component
/*!
 * Store the amount of health an entity has
 */
class health final : public component {
    public:
        /*!
         * Health constructor.
         * \param h Health value.
         * \param hm Max health value.
         * \return void
         */
        inline health(const int h, const int hm) :
        hp(h), hp_max(hm) {};

        int hp, hp_max;
};

} //  namespace cmp

} //  namespace wte

#endif
