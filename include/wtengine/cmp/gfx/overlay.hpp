/*!
 * WTEngine | File:  overlay.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_OVERLAY_HPP
#define WTE_CMP_OVERLAY_HPP

#include <string>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "wtengine/cmp/gfx/animator.hpp"
#include "wtengine/cmp/gfx/bitmap_map.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class overlay
 * \brief Component for storing an overlay image and defining its animation process.
 */
class overlay final : public animator, public bitmap_map {
    public:
        /*!
         * \brief Overlay constructor.
         * 
         * \param w Overlay width in pixels.
         * \param h Overlay height in pixels.
         * \param x Horizontal location of the overlay in pixels.
         * \param y Verticle location of the overlay in pixels.
         * \param l Layer for sorting.
         * \param func Function to define what is displayed in the overlay.
         */
        overlay(const int w, const int h, const float x, const float y,
                       const std::size_t l, void func(const entity_id&, mgr::entity_manager&, const int64_t&));

        /*!
         * \brief Overlay destructor.
         */
        ~overlay();

        /*!
         * \brief Reload the overlay bitmap.
         * 
         * Called when the screen is updated.
         */
        void reload_overlay_bitmap(void);

        /*!
         * \brief Draw text on the overlay.
         * 
         * \param txt Text to be displayed.
         * \param color Allegro color object.
         * \param x Horizontal location of the text.
         * \param y Verticle location of the text.
         * \param f Text flags for drawing - see Allegro docs on al_draw_text.
         */
        void draw_text(const std::string& txt, const ALLEGRO_COLOR& color,
                              const float& x, const float& y, const int& f);

        /*!
         * \brief Get X position.
         * 
         * \return X position.
         */
        const float get_pos_x(void) const;

        /*!
         * \brief Get Y position.
         * 
         * \return Y position.
         */
        const float get_pos_y(void) const;

        /*!
         * \brief Set the font used by the overlay.
         * 
         * \param font Allegro font object to be used.
         */
        void set_font(ALLEGRO_FONT* font);

    private:
        ALLEGRO_FONT* overlay_font;

        int overlay_w, overlay_h;
        float pos_x, pos_y;
};

} //  namespace cmp

} //  namespace wte

#endif
