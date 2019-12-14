/*
  Menu
  By:  Matthew Evans
  File:  menu.hpp

  See LICENSE.txt for copyright information

  A menu
*/

#ifndef WTE_MENU_MENU_HPP
#define WTE_MENU_MENU_HPP

#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

class menu {
    public:
        menu(std::string);

    private:
        std::string menu_name;
        std::vector<menu_item> items;
        ALLEGRO_BITMAP *background_bitmap;
};

//!
/*!
*/
inline menu::menu(std::string name) {
    menu_name = name;
}

}  // end namespace mnu

}  // end namespace wte

#endif
