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
        animator(func), pos_x(x), pos_y(y), layer(l) {
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
         * Override < operator to sort by layer value.
         * 
         * \param a Object to compare to.
         */
        bool operator<(const overlay& a) const {
            return layer < a.layer;
        };

        /*!
         *
         */
        inline void set_font(ALLEGRO_FONT* font) { overlay_font = font; };

        /*!
         *
         */
        inline void set_text(std::string txt, ALLEGRO_COLOR color) {
            //
        };

        ALLEGRO_BITMAP* overlay_bitmap;
        float pos_x, pos_y;
        std::size_t layer;

    private:
        ALLEGRO_FONT* overlay_font;
};

} //  namespace cmp

} //  namespace wte

#endif
