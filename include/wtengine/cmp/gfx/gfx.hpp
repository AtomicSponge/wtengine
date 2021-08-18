/*!
 * WTEngine | File:  animator.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_ANIMATOR_HPP
#define WTE_CMP_ANIMATOR_HPP

#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/cmp/component.hpp"

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class gfx
 * \brief Interface class for components that display graphics.
 */
class gfx : public component {
    public:
        virtual ~gfx() = default;

        /*!
         * \brief Overload < operator to sort by layer value.
         * \param a Object to compare to.
         * \return True if <, false if >
         */
        const bool operator<(const gfx& a) const {
            return layer < a.layer;
        };

        /*!
         * \brief Run animaton function wrapper.
         * \param e_id ID of the entity calling the wrapper.
         */
        void run(const entity_id& e_id);

        /*!
         * \brief Set drawing to the internal bitmap.
         */
        void set_drawing(void);

        /*!
         * \brief Return the internal bitmap.
         * \return The internal bitmap.
         */
        ALLEGRO_BITMAP* get_bitmap(void) const;

        /*!
         * \brief Set a tint color.
         * \param c Allegro color.
         */
        void set_tint(const ALLEGRO_COLOR& c);

        /*!
         * \brief Get the tint color.
         * \return Allegro color.
         */
        const ALLEGRO_COLOR get_tint(void) const;

        /*!
         * \brief Clear tint color.
         */
        void clear_tint(void);

        /*!
         * \brief Check if the bitmap should be drawn tinted.
         * \return True if tint set, false if tint cleared.
         */
        const bool draw_tinted(void) const;

        /*!
         * \brief Show the entity.
         */
        void show(void);

        /*!
         * \brief Hide the entity.
         */
        void hide(void);

        /*!
         * \brief Check if visible.
         * \return True if visible, false if not.
         */
        const bool is_visible(void) const;

        /*!
         * \brief Set to draw rotated.
         */
        void is_roated(void);

        /*!
         * \brief Set to not draw rotated.
         */
        void not_roated(void);

        /*!
         * \brief Check if should be drawn rotated.
         * \return True if draw rotated, false if not.
         */
        const bool draw_rotated(void) const;

        /*!
         * \brief Set direction of rotation.
         * \param d Direction in radians.
         */
        void set_direction(const float& d);

        /*!
         * \brief Get direction of rotation.
         * \return Direction in radians.
         */
        const float get_direction(void) const;

    protected:
        /*!
         * \brief Extend to create a gfx component.
         * \param bmp Bitmap asset to use.
         * \param l Layer position.
         * \param func Animation function.
         */
        gfx(
            wte_asset<al_bitmap> bmp,
            const std::size_t& l,
            const std::function<void(const entity_id&)>& func
        );

        //!  Stores the bitmap used by the animator.
        wte_asset<al_bitmap> internal_bitmap;

    private:
        std::size_t layer;          //  Layer position.
        bool tint_set;              //  Flag to set tint.
        bool visible;               //  Flag to set visibility.
        ALLEGRO_COLOR tint_color;   //  Color of tint.

        float direction;    //  Direction to draw when rotated.
        bool rotated;       //  Rotation flag.

        //  Animation function.
        std::function<void(const entity_id&)> animate;
};

} //  namespace cmp

} //  namespace wte

#endif
