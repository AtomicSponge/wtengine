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

#include "../message.hpp"

namespace wte
{

namespace mnu
{

typedef std::vector<std::pair<std::string, msg::message>> menu_option;

class menu_item {
    public:
        menu_item();

    private:
        std::string item_text;
        menu_option options;
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
