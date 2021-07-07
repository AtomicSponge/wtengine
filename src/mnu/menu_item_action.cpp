/*!
 * WTEngine | File:  menu_item_action.cpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/menu_item_action.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
menu_item_action::menu_item_action(const std::string l, const std::string c, const std::string a) :
    menu_item(l), cmd(c), arg(a) {};

/*
 *
 */
menu_item_action::menu_item_action(const std::string l, const std::string c) :
    menu_item(l), cmd(c), arg("") {};

/*
 *
 */
menu_item_action::~menu_item_action() {};

/*
 *
 */
const message menu_item_action::on_select(void) { return message("system", cmd, arg); };

/*
 *
 */
const std::vector<std::string> menu_item_action::get_text(void) const { return { get_label() }; };

}  // end namespace mnu

}  // end namespace wte
