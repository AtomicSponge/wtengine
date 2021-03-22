/*!
 * WTEngine | File:  velocity.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_VELOCITY_HPP
#define WTE_CMP_VELOCITY_HPP

#include "wtengine/cmp/component.hpp"

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
         * \brief Velocity constructor.
         */
        velocity(void);

        /*!
         * \brief Velocity constructor.
         * 
         * \param v Velocity for X and Y.
         */
        velocity(const float v);

        /*!
         * \brief Velocity constructor.
         * 
         * \param xv X velocity.
         * \param yv Y velocity.
         */
        velocity(const float xv, const float yv);

        /*!
         * \brief Set X velocity.
         * 
         * \param xv X velocity.
         */
        void set_x_vel(const float& xv);

        /*!
         * \brief Set Y velocity.
         * 
         * \param yv Y velocity.
         */
        void set_y_vel(const float& yv);

        /*!
         * \brief Set velocity for X and Y.
         * 
         * \param v Velocity
         */
        void set_velocity(const float& v);

        /*!
         * \brief Get X velocity.
         * 
         * \return X velocity.
         */
        const float get_x_vel(void) const;

        /*!
         * \brief Get Y velocity.
         * 
         * \return Y velocity.
         */
        const float get_y_vel(void) const;

    private:
        float x_vel, y_vel;
};

} //  namespace cmp

} //  namespace wte

#endif
