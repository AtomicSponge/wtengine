/*
  Menu manager
  By:  Matthew Evans
  File:  menu_manager.hpp

  See LICENSE.txt for copyright information

  Menu manager
*/

#ifndef WTE_MENU_MENU_MANAGER_HPP
#define WTE_MENU_MENU_MANAGER_HPP

#include <stack>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "menu\menu.hpp"

namespace wte
{

namespace mnu
{

class menu_manager {
    public:
        //

    private:
        std::vector<menu> menus;
        std::stack<menu> opened_menus;
};

}  // end namespace mnu

}  // end namespace wte

#endif
