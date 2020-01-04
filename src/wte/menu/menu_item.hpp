/*
  Menu item
  By:  Matthew Evans
  File:  menu_item.hpp

  See LICENSE.txt for copyright information

  A menu item
*/

#ifndef WTE_MENU_MENU_ITEM_HPP
#define WTE_MENU_MENU_ITEM_HPP

#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

namespace wte
{

namespace mnu
{

class menu_item {
    public:
        menu_item();

    private:
        std::string item_text;
        std::vector<int> options;
        std::vector<std::string> option_text;
};

//!
/*!
*/
inline menu_item::menu_item() {
    //
}

}  // end namespace mnu

}  // end namespace wte

#endif
