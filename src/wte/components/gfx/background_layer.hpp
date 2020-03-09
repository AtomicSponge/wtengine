/*
  WTEngine
  By:  Matthew Evans
  File:  background_layer.hpp

  See LICENSE.txt for copyright information

  Background layer component
*/

#ifndef WTE_CMP_BACKGROUND_LAYER_HPP
#define WTE_CMP_BACKGROUND_LAYER_HPP

#include <allegro5/allegro.h>

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Background Layer component
/*!
  Used to render different backgrounds in order
*/
class background_layer : public component {
    public:
        inline background_layer(int l) : layer(l) {};

        int layer;
};

} //  namespace cmp

} //  namespace wte

#endif
