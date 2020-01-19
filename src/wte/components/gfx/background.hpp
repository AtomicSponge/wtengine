/*
  WTEngine
  By:  Matthew Evans
  File:  background.hpp

  See LICENSE.txt for copyright information

  Background component
*/

#ifndef WTE_ECS_COMPONENT_BACKGROUND_HPP
#define WTE_ECS_COMPONENT_BACKGROUND_HPP

#include <allegro5/allegro.h>

#include "../../wte_globals.hpp"
#include "../component.hpp"

namespace wte
{

namespace ecs
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
    background_bitmap = al_create_bitmap(screen_width, screen_height);
}

inline background::~background() {
    al_destroy_bitmap(background_bitmap);
}

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
