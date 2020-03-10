/*!
 * \brief WTEngine | File:  sprite.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Sprite component.
 */

#ifndef WTE_CMP_SPRITE_HPP
#define WTE_CMP_SPRITE_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Sprite component
/*!
  WIP
*/
class sprite : public component {
    public:
        ~sprite();

        ALLEGRO_BITMAP *sprite_bitmap;
        float sprite_height, sprite_width, draw_offset;
        unsigned int current_frame, speed;
};

inline sprite::~sprite() {
    al_destroy_bitmap(sprite_bitmap);
}

} //  namespace cmp

} //  namespace wte

#endif
