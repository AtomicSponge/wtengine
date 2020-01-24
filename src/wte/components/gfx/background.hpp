/*
  WTEngine
  By:  Matthew Evans
  File:  background.hpp

  See LICENSE.txt for copyright information

  Background component
*/

#ifndef WTE_CMP_BACKGROUND_HPP
#define WTE_CMP_BACKGROUND_HPP

#include <allegro5/allegro.h>

#include "../../wte_globals.hpp"
#include "../../wte_config.hpp"
#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Background component
/*!
  Store the background bitmap
*/
class background : public component {
    public:
        background();
        ~background();

        ALLEGRO_BITMAP *background_bitmap;
};

inline background::background() {
    background_bitmap = al_create_bitmap(wte_config::screen_width, wte_config::screen_height);
}

inline background::~background() {
    al_destroy_bitmap(background_bitmap);
}

} //  namespace cmp

} //  namespace wte

#endif
