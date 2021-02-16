/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  health.hpp
 *
 * See LICENSE.txt for copyright information
 *
 * Health component.
 */

#ifndef WTEDEMO_CMP_HEALTH_HPP
#define WTEDEMO_CMP_HEALTH_HPP

#include <wte/wte.hpp>

/*
 * Store the amount of health an entity has
 */
class health final : public wte::cmp::component {
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

#endif
