/*!
 * \brief WTEngine | File:  background.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Background component.
 */

#ifndef WTE_CMP_BACKGROUND_HPP
#define WTE_CMP_BACKGROUND_HPP

#include <allegro5/allegro.h>

#include "../../engine_cfg_map.hpp"
#include "animator.hpp"

namespace wte
{

namespace cmp
{

//! Background component
/*!
 * Store the background bitmap
 */
class background final : public animator {
    public:
        /*!
         * Background
         * \param void
         * \return void
         */
        inline background(std::size_t l) : layer(l) {
            background_bitmap = al_create_bitmap(engine_cfg::get<int>("screen_width"),
                                                 engine_cfg::get<int>("screen_height"));
        }

        /*!
         * Background
         * \param void
         * \return void
         */
        inline background(std::size_t l, void func(entity, mgr::entity_manager&, int64_t)) :
        animator(func),  layer(l) {
            background_bitmap = al_create_bitmap(engine_cfg::get<int>("screen_width"),
                                                 engine_cfg::get<int>("screen_height"));
        }

        /*!
         * Background
         * \param void
         * \return void
         */
        inline ~background() {
            al_destroy_bitmap(background_bitmap);
        }

        /*!
         * Override < operator to sort by layer value.
         * 
         * \param a Object to compare to.
         */
        bool operator<(const background& a) const {
            return layer < a.layer;
        };

        ALLEGRO_BITMAP *background_bitmap;
        std::size_t layer;
};

} //  namespace cmp

} //  namespace wte

#endif
