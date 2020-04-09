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

        std::vector<std::string> get_text(void);
        void do_item(void);
    
    private:
        std::string cmd;
        std::string arg;
};

/*!
*/
inline std::vector<std::string> menu_item_action::get_text(void) {
    return {};
}

/*!
*/
inline void menu_item_action::do_item(void) {
    //
}

}  // end namespace mnu

}  // end namespace wte

#endif
