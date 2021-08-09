/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  damage.hpp
 *
 * See LICENSE.txt for copyright information
 *
 * Damage component.
 */

#ifndef WTEDEMO_CMP_DAMAGE_HPP
#define WTEDEMO_CMP_DAMAGE_HPP

#include <wtengine/wtengine.hpp>

/*
 * Store the amount of damage an entity can do
 */
class damage final : public wte::cmp::component {
    public:
        /*!
         * Damage constructor.
         * \param d Damage amount.
         * \return void
         */
        inline damage(const int& d) : dmg(d) {};

        int dmg;
};

#endif
