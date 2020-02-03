/*
  WTEngine
  By:  Matthew Evans
  File:  menu_item_action.hpp

  See LICENSE.txt for copyright information

  Action menu item
*/

#ifndef WTE_MENU_MENU_ITEM_ACTION_HPP
#define WTE_MENU_MENU_ITEM_ACTION_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!
/*!
*/
class menu_item_action final : public menu_item {
    public:
        inline menu_item_action() {};
        inline ~menu_item_action() {};

        std::vector<std::string> get_text(void);
        void do_item(void);
};

}  // end namespace mnu

}  // end namespace wte

#endif
