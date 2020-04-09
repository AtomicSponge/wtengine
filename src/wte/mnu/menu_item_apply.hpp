/*!
 * \brief WTEngine | File:  menu_item_apply.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Setting menu item.
 */

#ifndef WTE_MENU_MENU_ITEM_APPLY_HPP
#define WTE_MENU_MENU_ITEM_APPLY_HPP

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
class menu_item_apply final : public menu_item {
    public:
        inline menu_item_apply(std::string c, std::string a) : cmd(c), arg(a) {};
        inline ~menu_item_apply() {};

        message on_select(void) override { return message("system", cmd, arg); };

        std::vector<std::string> get_text(void) { return {}; };

    private:
        std::string cmd;
        std::string arg;
};

}  // end namespace mnu

}  // end namespace wte

#endif
