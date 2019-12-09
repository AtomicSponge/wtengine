/*
  WTEngine
  By:  Matthew Evans
  File:  sprite.hpp

  See LICENSE.txt for copyright information

  Sprite component
*/

#ifndef WTE_ECS_COMPONENT_SPRITE_HPP
#define WTE_ECS_COMPONENT_SPRITE_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Sprite component
/*!
  WIP
*/
class sprite : public component {
    public:
        ALLEGRO_BITMAP *sprite_bitmap;
        float sprite_height, sprite_width, draw_offset;
        unsigned int current_frame, speed;

        ~sprite();
};

inline sprite::~sprite() {
    al_destroy_bitmap(sprite_bitmap);
}

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
