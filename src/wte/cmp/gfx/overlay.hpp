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
         * overlay
         * \param void
         * \return void
         */
        inline overlay(int w, int h, float x, float y, std::size_t l, void func(entity, mgr::entity_manager&, int64_t)) :
        animator(l, func), pos_x(x), pos_y(y) {
            overlay_bitmap = al_create_bitmap(w, h);
            overlay_font = NULL;
        };

        /*!
         * overlay
         * \param void
         * \return void
         */
        inline ~overlay() {
            al_destroy_bitmap(overlay_bitmap);
            al_destroy_font(overlay_font);
        };

        /*!
         * overlay
         * \param void
         * \return void
         */
        inline void set_font(ALLEGRO_FONT* font) { overlay_font = font; };

        /*!
         * overlay
         * \param void
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
