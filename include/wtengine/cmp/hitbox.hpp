/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_HITBOX_HPP
#define WTE_CMP_HITBOX_HPP

#include "wtengine/cmp/component.hpp"

namespace wte
{

namespace cmp
{

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

        ~hitbox() = default;

        /*!
         * \brief Get hitbox width.
         * \return Width of hitbox.
         */
        const float get_width(void) const;

        /*!
         * \brief Get hitbox height.
         * \return Height of hitbox.
         */
        const float get_height(void) const;

        /*!
         * \brief Set width of hitbox.
         * \param w New width.
         */
        void set_width(const float& w);

        /*!
         * \brief Set height of hitbox.
         * \param h New height.
         */
        void set_height(const float& h);

        /*!
         * \brief Check if solid.
         * \return True if solid, false if not.
         */
        const bool is_solid(void) const;

        /*!
         * \brief Make entity solid (enable).
         */
        void make_solid(void);

        /*!
         * \brief Make entity fluid (disable).
         */
        void make_fluid(void);

    private:
        float width, height;    //  Width & height of hitbox.
        bool solid;             //  Solid (enabled) flag.
};

} //  namespace cmp

} //  namespace wte

#endif
