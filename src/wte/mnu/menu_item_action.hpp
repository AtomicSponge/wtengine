/*!
 * \brief WTEngine | File:  menu_item_action.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Action menu item.
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

//!  Menu item action.
/*!
 * Menu item that runs a message
*/
class menu_item_action final : public menu_item {
    public:
        inline menu_item_action(std::string c, std::string a) : cmd(c), arg(a) {};
        inline ~menu_item_action() {};

        message on_select(void) override { return message("system", cmd, arg); };

        std::vector<std::string> get_text(void) override { return {}; };

    private:
        std::string cmd;
        std::string arg;
};

}  // end namespace mnu

}  // end namespace wte

#endif
