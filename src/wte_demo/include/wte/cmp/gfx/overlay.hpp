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
#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Overlay component
/*!
 * Store the overlay bitmap
 */
class overlay : public component {
    public:
        /*!
         * overlay
         * \param void
         * \return void
         */
        inline overlay(std::size_t l) : layer(l) {
            overlay_bitmap = al_create_bitmap(engine_cfg::get<int>("screen_width"),
                                                 engine_cfg::get<int>("screen_height"));
        }

        /*!
         * overlay
         * \param void
         * \return void
         */
        inline ~overlay() {
            al_destroy_bitmap(overlay_bitmap);
        }

        /*!
         * Override < operator to sort by layer value.
         * 
         * \param a Object to compare to.
         */
        bool operator<(const overlay& a) const {
            return layer < a.layer;
        };

        ALLEGRO_BITMAP *overlay_bitmap;
        std::size_t layer;
};

} //  namespace cmp

} //  namespace wte

#endif
