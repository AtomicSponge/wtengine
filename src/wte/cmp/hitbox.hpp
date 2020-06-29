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

/*!
 * \class Hitbox component
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

        /*!
         *
         */
        inline const float get_width(void) const {
            return width;
        };

        /*!
         *
         */
        inline const float get_height(void) const {
            return height;
        };

        /*!
         *
         */
        inline void set_width(const float& w) {
            width = w;
        };

        /*!
         *
         */
        inline void set_height(const float& h) {
            height = h;
        };

        /*!
         *
         */
        inline const bool is_solid(void) const {
            return solid;
        };

        /*!
         *
         */
        inline void make_solid(void) {
            solid = true;
        };

        /*!
         *
         */
        inline void make_fluid(void) {
            solid = false;
        };

    private:
        float width, height;
        bool solid;
};

} //  namespace cmp

} //  namespace wte

#endif
