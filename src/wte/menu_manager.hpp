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

typedef unsigned int menu_id;

class menu_manager {
    public:
        const menu_id new_menu(void);
        const bool add_item(const menu_id, const menu_item);

    private:
        std::vector<menu> menus;
        std::stack<menu> opened_menus;
};

inline const menu_id menu_manager::new_menu(void) {
    menu_id next_id;
    return next_id;
}

inline const bool add_item(const menu_id id, const menu_item item) {
    return true;
}

}  // end namespace mnu

}  // end namespace wte

#endif
