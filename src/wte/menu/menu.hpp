/*
  Menu
  By:  Matthew Evans
  File:  menu.hpp

  See LICENSE.txt for copyright information

  A menu
*/

#ifndef WTE_MENU_MENU_HPP
#define WTE_MENU_MENU_HPP

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
        //

    private:
        std::vector<menu_item> items;
};

}  // end namespace mnu

}  // end namespace wte

#endif
