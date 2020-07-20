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
 * \class hitbox
 * \brief Store the width and height for an entity's hitbox
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
         * Get hitbox width.
         * \param void
         * \return Width of hitbox.
         */
        inline const float get_width(void) const {
            return width;
        };

        /*!
         * Get hitbox height.
         * \param void
         * \return Height of hitbox.
         */
        inline const float get_height(void) const {
            return height;
        };

        /*!
         * Set width of hitbox.
         * \param w New width.
         * \return void
         */
        inline void set_width(const float& w) {
            width = w;
        };

        /*!
         * Set height of hitbox.
         * \param h New height.
         * \return void
         */
        inline void set_height(const float& h) {
            height = h;
        };

        /*!
         * Check if solid.
         * \param void
         * \return True if solid, false if not.
         */
        inline const bool is_solid(void) const {
            return solid;
        };

        /*!
         * \brief Make entity solid.
         * Sets internal flag to true.
         * \param void
         * \return void
         */
        inline void make_solid(void) {
            solid = true;
        };

        /*!
         * \brief Make entity not solid.
         * Sets internal flag to false.
         * \param void
         * \return void
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
