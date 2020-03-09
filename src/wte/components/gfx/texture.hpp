/*
  WTEngine
  By:  Matthew Evans
  File:  texture.hpp

  See LICENSE.txt for copyright information

  Texture component
*/

#ifndef WTE_CMP_TEXTURE_HPP
#define WTE_CMP_TEXTURE_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Texture component
/*!
  WIP
*/
class texture : public component {
    public:
        ~texture();

        ALLEGRO_BITMAP *texture_bitmap;
};

inline texture::~texture() {
    al_destroy_bitmap(texture_bitmap);
}

} //  namespace cmp

} //  namespace wte

#endif
