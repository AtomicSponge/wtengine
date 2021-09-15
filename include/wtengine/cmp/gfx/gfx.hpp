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

namespace wte::mgr {
    class renderer;
}

namespace wte::sys {
    class animate;
}

namespace wte::cmp {

/*!
 * \class gfx
 * \brief Interface class for components that display graphics.
 */
class gfx : public component {
    friend class mgr::renderer;
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

        std::size_t layer;     //!<  Layer position.
        bool visible;          //!<  Flag to set visibility.
        bool rotated;          //!<  Rotation flag.
        float direction;       //!<  Direction to draw when rotated.
        float scale_factor_x;  //!<  X scale factor.
        float scale_factor_y;  //!<  Y scale factor.

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
        wte_asset<al_bitmap> _bitmap;

    private:
        bool tint_set;              //  Flag to set tint.
        ALLEGRO_COLOR tint_color;   //  Color of tint.

        //  Animation function.
        const std::function<void(const entity_id&)> animate;
};

}  //  end namespace wte::cmp

#endif
