/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_CMP_MOTION_HPP)
#define SLV_CMP_MOTION_HPP

#include "silvergun/cmp/component.hpp"

namespace slv::cmp {

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
    ) : direction(d), x_vel(xv), y_vel(yv) {};

    motion() = delete;    //  Delete default constructor.
    ~motion() = default;  //  Default destructor.

    float direction;  //!<  Angle of direction.
    float x_vel;      //!<  X velocity.
    float y_vel;      //!<  Y velocity.
};

}

#endif
