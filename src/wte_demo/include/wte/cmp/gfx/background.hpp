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
#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Background component
/*!
 * Store the background bitmap
 */
class background : public component {
    public:
        background();
        ~background();

        ALLEGRO_BITMAP *background_bitmap;
};

inline background::background() {
    background_bitmap = al_create_bitmap(engine_cfg::get<int>("screen_width"), engine_cfg::get<int>("screen_height"));
}

inline background::~background() {
    al_destroy_bitmap(background_bitmap);
}

} //  namespace cmp

} //  namespace wte

#endif
