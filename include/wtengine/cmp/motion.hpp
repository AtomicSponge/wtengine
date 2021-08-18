/*!
 * WTEngine | File:  motion.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_MOTION_HPP
#define WTE_CMP_MOTION_HPP

#include "wtengine/cmp/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class motion
 * \brief Store motion information (velocity and direction) of an entity.
 */
class motion final : public component {
    public:
        /*!
         * \brief Create a new Motion component.
         */
        motion(void);

        /*!
         * \brief Create a new Motion component with set direction and velocity.
         * \param d Direction in radians.
         * \param v Velocity for X and Y.
         */
        motion(
            const float& d,
            const float& v
        );

        /*!
         * \brief Create a new Motion component with set direction and velocity.
         * \param d Direction in radians.
         * \param xv X velocity.
         * \param yv Y velocity.
         */
        motion(
            const float& d,
            const float& xv,
            const float& yv
        );

        /*!
         * \brief
         */
        void set_direction(const float& d);

        /*!
         * \brief
         */
        const float get_direction(void) const;

        /*!
         * \brief Set X velocity.
         * \param xv X velocity.
         */
        void set_x_vel(const float& xv);

        /*!
         * \brief Set Y velocity.
         * \param yv Y velocity.
         */
        void set_y_vel(const float& yv);

        /*!
         * \brief Get X velocity.
         * \return X velocity.
         */
        const float get_x_vel(void) const;

        /*!
         * \brief Get Y velocity.
         * \return Y velocity.
         */
        const float get_y_vel(void) const;

        /*!
         * \brief Set velocity for X & Y.
         * \param v The new velocity value.
         */
        void set_velocity(const float& v);

        /*!
         * \brief Get velocity for X & Y.
         * \return The velocity value.
         */
        const float get_velocity(void) const;

    private:
        float direction;        //  Angle of direction.
        float x_vel, y_vel;     //  X & Y velocity.
};

} //  namespace cmp

} //  namespace wte

#endif
