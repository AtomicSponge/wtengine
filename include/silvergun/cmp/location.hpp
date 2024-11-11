/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_LOCATION_HPP)
#define WTE_CMP_LOCATION_HPP

#include "silvergun/cmp/component.hpp"

namespace wte::cmp {

/*!
 * \class location
 * \brief Store the X/Y location of an entity in the arena.
 */
class location final : public component {
  public:
    /*!
     * \brief Create a new Location component.
     * \param x Horizontal location of the entity.
     * \param y Vertical location of the entity.
     */
    location(
      const float& x,
      const float& y
    ) : pos_x(x), pos_y(y) {};

    location() = delete;    //  Delete default constructor.
    ~location() = default;  //  Default destructor.

    float pos_x;  //!<  Entity X location.
    float pos_y;  //!<  Entity Y location.
};

} //  namespace wte::cmp

#endif
