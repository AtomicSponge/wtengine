/*!
 * \brief WTEngine | File:  hitbox.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Hitbox component.
 */

#ifndef WTE_CMP_HITBOX_HPP
#define WTE_CMP_HITBOX_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Hitbox component
/*!
 * Store the width and height for an entity's hitbox
 */
class hitbox final : public component {
    public:
        /*!
         * \brief Hitbox constructor.
         * Call this to have the hitbox enabled by default.
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         * \return void
         */
        inline hitbox(float w, float h) : width(w), height(h), solid(true) {};

        /*!
         * \brief Hitbox constructor.
         * Call this to set if the hitbox is enabled or not.
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         * \param s Boolean value for if the hitbox is enabled.
         * \return void
         */
        inline hitbox(float w, float h, bool s) : width(w), height(h), solid(s) {};

        float width, height;
        bool solid;
};

} //  namespace cmp

} //  namespace wte

#endif
