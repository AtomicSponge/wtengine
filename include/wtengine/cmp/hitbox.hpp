/*!
 * WTEngine | File:  hitbox.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
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
         * \brief Hitbox constructor.
         * 
         * Call this to have the hitbox enabled by default.
         * 
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         */
        hitbox(float w, float h);

        /*!
         * \brief Hitbox constructor.
         * 
         * Call this to set if the hitbox is enabled or not.
         * 
         * \param w Width of the hitbox in pixels.
         * \param h Height of the hitbox in pixels.
         * \param s Boolean value for if the hitbox is enabled.
         */
        hitbox(float w, float h, bool s);

        /*!
         * \brief Get hitbox width.
         * 
         * \return Width of hitbox.
         */
        const float get_width(void) const;

        /*!
         * \brief Get hitbox height.
         * 
         * \return Height of hitbox.
         */
        const float get_height(void) const;

        /*!
         * \brief Set width of hitbox.
         * 
         * \param w New width.
         */
        void set_width(const float& w);

        /*!
         * \brief Set height of hitbox.
         * 
         * \param h New height.
         */
        void set_height(const float& h);

        /*!
         * \brief Check if solid.
         * 
         * \return True if solid, false if not.
         */
        const bool is_solid(void) const;

        /*!
         * \brief Make entity solid.
         * 
         * Sets internal flag to true.
         */
        void make_solid(void);

        /*!
         * \brief Make entity not solid.
         * 
         * Sets internal flag to false.
         */
        void make_fluid(void);

    private:
        float width, height;
        bool solid;
};

} //  namespace cmp

} //  namespace wte

#endif
