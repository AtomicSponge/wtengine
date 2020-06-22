/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  health.hpp
 *
 * See LICENSE.txt for copyright information
 *
 * Velocity component.
 */

#ifndef WTEDEMO_CMP_VELOCITY_HPP
#define WTEDEMO_CMP_VELOCITY_HPP

#include <wte/wte.hpp>

/*
 * Store the movement velocity of an entity
 */
class velocity final : public wte::cmp::component {
    public:
        /*!
         * Velocity constructor.
         * \param v Speed value for entity.
         * \return void
         */
        inline velocity(float v) : speed(v) {};

        float speed;
};

#endif
