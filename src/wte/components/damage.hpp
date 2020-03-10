/*!
 * \brief WTEngine | File:  damage.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Damage component.
 */

#ifndef WTE_CMP_DAMAGE_HPP
#define WTE_CMP_DAMAGE_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Damage component
/*!
  Store the amount of damage an entity can do
*/
class damage final : public component {
    public:
        inline damage(int d) : dmg(d) {};

        int dmg;
};

} //  namespace cmp

} //  namespace wte

#endif
