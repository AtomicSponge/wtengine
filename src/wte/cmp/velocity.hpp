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

/*!
 * \class Velocity component.
 * Store the movement velocity of an entity.
 */
class velocity final : public component {
    public:
        /*!
         * Velocity constructor.
         * \param v Speed value for entity.
         * \return void
         */
        inline velocity(const float v) : speed(v) {};

        /*!
         * Get entity speed.
         * \param void
         * \return Speed value.
         */
        inline const float get_speed(void) const {
            return speed;
        };

        /*!
         * Set entity speed.
         * \param s New speed value.
         * \return void
         */
        inline void set_speed(const float& s) {
            speed = s;
        };

    private:
        float speed;
};

} //  namespace cmp

} //  namespace wte

#endif
