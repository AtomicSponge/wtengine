/*!
 * \brief WTEngine | File:  overlay.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Overlay component.
 */

#ifndef WTE_CMP_OVERLAY_HPP
#define WTE_CMP_OVERLAY_HPP

#include <allegro5/allegro.h>

#include "../../engine_cfg_map.hpp"
#include "animator.hpp"

namespace wte
{

namespace cmp
{

//! Overlay component
/*!
 * Store the overlay bitmap
 */
class overlay final : public animator {
    public:
        /*!
         * Overlay constructor.
         * \param w Overlay width in pixels.
         * \param h Overlay height in pixels.
         * \param x Horizontal location of the overlay in pixels.
         * \param y Verticle location of the overlay in pixels.
         * \param l Layer for sorting.
         * \param func Function to define what is displayed in the overlay.
         * \return void
         */
        inline overlay(int w, int h, float x, float y, std::size_t l, void func(entity, mgr::entity_manager&, int64_t)) :
        animator(l, func), pos_x(x), pos_y(y) {
            overlay_bitmap = al_create_bitmap(w, h);
            overlay_font = NULL;
        };

        /*!
         * Overlay destructor.
         * \param void
         * \return void
         */
        inline ~overlay() {
            al_destroy_bitmap(overlay_bitmap);
            al_destroy_font(overlay_font);
        };

        /*!
         * Set the font used by the overlay.
         * \param font Allegro font object to be used.
         * \return void
         */
        inline void set_font(ALLEGRO_FONT* font) { overlay_font = font; };

        /*!
         * Set text on the overlay.
         * \param txt Text to be displayed.
         * \param color Allegro color object.
         * \param x Horizontal location of the text.
         * \param y Verticle location of the text.
         * \param f Text flags for drawing - see Allegro docs on al_draw_text.
         * \return void
         */
        inline void set_text(const std::string txt, const ALLEGRO_COLOR color, const float x, const float y, const int f) {
            al_draw_text(overlay_font, color, x, y, f, txt.c_str());
        };

        ALLEGRO_BITMAP* overlay_bitmap;
        float pos_x, pos_y;

    private:
        ALLEGRO_FONT* overlay_font;
};

} //  namespace cmp

} //  namespace wte

#endif
