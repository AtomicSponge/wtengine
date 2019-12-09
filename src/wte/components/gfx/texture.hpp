/*
  WTEngine
  By:  Matthew Evans
  File:  texture.hpp

  See LICENSE.txt for copyright information

  Texture component
*/

#ifndef WTE_ECS_COMPONENT_TEXTURE_HPP
#define WTE_ECS_COMPONENT_TEXTURE_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace ecs
{

namespace cmp
{

//! Texture component
/*!
  WIP
*/
class texture : public component {
    public:
        ALLEGRO_BITMAP *texture_bitmap;

        ~texture();
};

inline texture::~texture() {
    al_destroy_bitmap(texture_bitmap);
}

} //  namespace cmp

} //  namespace ecs

} //  namespace wte

#endif
