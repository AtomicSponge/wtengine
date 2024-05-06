/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/cmp/motion.hpp"

namespace wte::cmp {

/*
 *
 */
motion::motion(
  const float& d,
  const float& xv,
  const float& yv
) : direction(d), x_vel(xv), y_vel(yv) {}

}  //  end namespace wte::cmp
