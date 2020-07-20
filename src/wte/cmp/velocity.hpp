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
 * \class velocity
 * \brief Store the movement velocity of an entity.
 */
class velocity final : public component {
    public:
        /*!
         * Velocity constructor.
         */
        inline velocity(void) : x_vel(0.0f), y_vel(0.0f) {};

        /*!
         * Velocity constructor.
         * \param v Velocity for X and Y.
         */
        inline velocity(const float v) : x_vel(v), y_vel(v) {};

        /*!
         * Velocity constructor.
         * \param xv X velocity.
         * \param yv Y velocity.
         */
        inline velocity(const float xv, const float yv) : x_vel(xv), y_vel(yv) {};

        /*!
         * Set X velocity.
         * \param xv X velocity.
         */
        inline void set_x_vel(const float& xv) {
            x_vel = xv;
        };

        /*!
         * Set Y velocity.
         * \param yv Y velocity.
         */
        inline void set_y_vel(const float& yv) {
            y_vel = yv;
        };

        /*!
         * Set velocity for X and Y.
         * \param v Velocity
         */
        inline void set_velocity(const float& v) {
            x_vel = v;
            y_vel = v;
        };

        /*!
         * Get X velocity.
         * \return X velocity.
         */
        inline const float get_x_vel(void) const {
            return x_vel;
        };

        /*!
         * Get X velocity.
         * \return Y velocity.
         */
        inline const float get_y_vel(void) const {
            return y_vel;
        };

    private:
        float x_vel, y_vel;
};

} //  namespace cmp

} //  namespace wte

#endif
