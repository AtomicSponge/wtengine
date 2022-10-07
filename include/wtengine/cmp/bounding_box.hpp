/*!
 * wtengine | File:  bounding_box.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_CMP_BOUNDING_BOX_HPP
#define WTE_CMP_BOUNDING_BOX_HPP

#include "wtengine/cmp/component.hpp"

namespace wte::cmp {

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
        );

        bounding_box() = delete;    //!<  Delete default constructor.
        ~bounding_box() = default;  //!<  Default destructor.

        float min_x;  //!<  Top left X position of bounding box.
        float min_y;  //!<  Top left Y position of bounding box.
        float max_x;  //!<  Bottom right X position of bounding box.
        float max_y;  //!<  Bottom right Y position of bounding box.
};

}  //  end namespace wte::cmp

#endif
