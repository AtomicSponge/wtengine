/*!
 * \brief WTEngine | File:  velocity.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Velocity component.
 */

#ifndef WTE_CMP_VELOCITY_HPP
#define WTE_CMP_VELOCITY_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Velocity component
/*!
 * Store the movement velocity of an entity
 */
class velocity final : public component {
    public:
        inline velocity(float v) : speed(v) {};

        float speed;
};

} //  namespace cmp

} //  namespace wte

#endif
