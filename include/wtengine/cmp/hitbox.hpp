/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_HITBOX_HPP
#define WTE_CMP_HITBOX_HPP

#include "wtengine/cmp/component.hpp"

namespace wte::cmp {

/*!
 * \class hitbox
 * \brief Component to add a hitbox for performing colisions on.
 */
class hitbox final : public component {
    public:
        /*!
         * \brief Create a new Hitbox component.
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         */
        hitbox(
            const float& w,
            const float& h
        );

        /*!
         * \brief Create a new Hitbox component, set solid flag.
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         * \param s Boolean value for if the hitbox is solid (enabled).
         */
        hitbox(
            const float& w,
            const float& h,
            const bool& s
        );

        ~hitbox() = default;  //!<  Default destructor.

        float width;   //!<  Width of the hitbox.
        float height;  //!<  Height of the hitbox.
        bool solid;    //!<  Solid (enabled) flag.
};

}  //  end namespace wte::cmp

#endif
