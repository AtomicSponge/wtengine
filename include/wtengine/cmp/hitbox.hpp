/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
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
         * \param t Team value for the hitbox.
         */
        hitbox(
            const float& w,
            const float& h,
            const std::size_t& t
        );

        /*!
         * \brief Create a new Hitbox component, set solid flag.
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         * \param t Team value for the hitbox.
         * \param s Boolean value for if the hitbox is solid (enabled).
         */
        hitbox(
            const float& w,
            const float& h,
            const std::size_t& t,
            const bool& s
        );

        hitbox() = delete;    //!<  Delete default constructor.
        ~hitbox() = default;  //!<  Default destructor.

        float width;       //!<  Width of the hitbox.
        float height;      //!<  Height of the hitbox.
        std::size_t team;  //!<  Team number.
        bool solid;        //!<  Solid (enabled) flag.
};

}  //  end namespace wte::cmp

#endif
