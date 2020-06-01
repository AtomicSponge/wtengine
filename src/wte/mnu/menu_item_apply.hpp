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
 *
 */
class menu_item_apply final : public menu_item {
    public:
        inline menu_item_apply() : menu_item("applier"), cmd("cancel"), arg("") {};
        inline ~menu_item_apply() {};

        /*!
         *
         */
        inline void on_left(void) override { if(cmd == "apply") cmd = "cancel"; };

        /*!
         *
         */
        inline void on_right(void) override { if(cmd == "cancel") cmd = "apply"; };

        /*!
         *
         */
        inline const message on_select(void) override { return message("menu", cmd, arg); };

        /*!
         *
         */
        inline const std::vector<std::string> get_text(void) override {
            if(cmd == "apply") return { "Cancel", "< Apply >" };
            return { "< Cancel >", "Apply" };
        };

    private:
        std::string cmd;
        std::string arg;
};

}  // end namespace mnu

}  // end namespace wte

#endif
