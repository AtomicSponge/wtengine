/*!
 * wtengine | File:  motion.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_CMP_MOTION_HPP
#define WTE_CMP_MOTION_HPP

#include "wtengine/cmp/component.hpp"

namespace wte::cmp {

/*!
 * \class motion
 * \brief Store motion information (velocity and direction) of an entity.
 */
class motion final : public component {
    public:
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

        motion() = delete;    //!<  Delete default constructor.
        ~motion() = default;  //!<  Default destructor.

        float direction;  //!<  Angle of direction.
        float x_vel;      //!<  X velocity.
        float y_vel;      //!<  Y velocity.
};

}  //  end namespace wte::cmp

#endif
