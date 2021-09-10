/*!
 * WTEngine | File:  gfx.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
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

namespace wte::sys {
    class animate;
}

namespace wte::cmp {

/*!
 * \class gfx
 * \brief Interface class for components that display graphics.
 */
class gfx : public component {
    friend class sys::animate;

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
         * \brief Set rotation.
         * \param r True to draw rotated, false to not.
         */
        void set_rotation(const bool& r);

        /*!
         * \brief Check if should be drawn rotated.
         * \return True if draw rotated, false if not.
         */
        const bool is_rotated(void) const;

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

        /*!
         * \brief Set the X scale factor.
         * \param sx New scale factor X value.
         */
        void set_scale_factor_x(const float& sx);

        /*!
         * \brief Set the Y scale factor.
         * \param sy New scale factor Y value.
         */
        void set_scale_factor_y(const float& sy);

        /*!
         * \brief Get the X scale factor.
         * \return scale_factor_x
         */
        const float get_scale_factor_x(void) const;

        /*!
         * \brief Get the Y scale factor.
         * \return scale_factor_y
         */
        const float get_scale_factor_y(void) const;

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
        /*
         * Run animaton function wrapper.
         */
        void run(const entity_id& e_id);

        std::size_t layer;          //  Layer position.
        bool tint_set;              //  Flag to set tint.
        bool visible;               //  Flag to set visibility.
        ALLEGRO_COLOR tint_color;   //  Color of tint.

        float direction;    //  Direction to draw when rotated.
        bool rotated;       //  Rotation flag.
        float scale_factor_x, scale_factor_y;   //  X/Y scale factor.

        //  Animation function.
        std::function<void(const entity_id&)> animate;
};

}  //  end namespace wte::cmp

#endif
