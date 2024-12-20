/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_CMP_BOUNDING_BOX_HPP)
#define SLV_CMP_BOUNDING_BOX_HPP

#include "silvergun/cmp/component.hpp"

namespace slv::cmp {

/*!
 * \class bounding_box
 * \brief Set a bounding box for an entity.
 * 
 * Entities can not move out of this box.
 */
class bounding_box final : public component {
  public:
    /*!
     * \brief Create a new Bounding Box component.
     * \param lx Left X
     * \param ly Left Y
     * \param rx Right X
     * \param ry Right Y
     */
    bounding_box(
      const float& lx,
      const float& ly,
      const float& rx,
      const float& ry
    ) : min_x(lx), min_y(ly), max_x(rx), max_y(ry) {};

    bounding_box() = delete;    //  Delete default constructor.
    ~bounding_box() = default;  //  Default destructor.

    float min_x;  //!<  Top left X position of bounding box.
    float min_y;  //!<  Top left Y position of bounding box.
    float max_x;  //!<  Bottom right X position of bounding box.
    float max_y;  //!<  Bottom right Y position of bounding box.
};

}

#endif
