/*!
 * WTEngine | File:  menu_item_apply.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/menu_item_apply.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
menu_item_apply::menu_item_apply() : menu_item("applier"), cmd("cancel") {};

/*
 *
 */
menu_item_apply::~menu_item_apply() {};

/*
 *
 */
void menu_item_apply::on_left(void) { cmd = "cancel"; };

/*
 *
 */
void menu_item_apply::on_right(void) { cmd = "apply"; };

/*
 *
 */
const message menu_item_apply::on_select(void) { return message("menu", cmd, ""); };

/*
 *
 */
const std::vector<std::string> menu_item_apply::get_text(void) const {
    if(cmd == "apply") return { "Cancel", "< Apply >" };
    return { "< Cancel >", "Apply" };
};

/*
 *
 */
void menu_item_apply::reset_to_default(void) { cmd = "cancel"; };

/*
 *
 */
void menu_item_apply::set_default(void) { cmd = "cancel"; };

}  // end namespace mnu

}  // end namespace wte
