/*!
 * WTEngine | File:  menu_item_toggle.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/menu_item_toggle.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
menu_item_toggle::menu_item_toggle(const std::string label,
                        const std::string con,
                        const std::string aon,
                        const std::string coff,
                        const std::string aoff,
                        bool func(void)) :
menu_item(label), cmd_on(con), cmd_off(coff), arg_on(aon), arg_off(aoff), defaulter(func) {
    toggled = default_val = defaulter();
}

/*
 *
 */
menu_item_toggle::menu_item_toggle(const std::string label,
                        const std::string con,
                        const std::string aon,
                        const std::string coff,
                        const std::string aoff,
                        const std::size_t st,
                        bool func(void)) :
menu_item(label, st), cmd_on(con), cmd_off(coff), arg_on(aon), arg_off(aoff), defaulter(func) {
    toggled = default_val = defaulter();
}

/*
 *
 */
menu_item_toggle::~menu_item_toggle() {}

/*
 *
 */
void menu_item_toggle::on_left(void) { toggled = true; }

/*
 *
 */
void menu_item_toggle::on_right(void) { toggled = false; }

/*
 *
 */
const std::vector<std::string> menu_item_toggle::get_text(void) const {
    if(toggled) return { get_label(), "< On >", "Off" };
    return { get_label(), "On", "< Off >" };
}

/*
 *
 */
const std::string menu_item_toggle::get_active_cmd(void) {
    if(toggled) return cmd_on;
    return cmd_off;
}

/*
 *
 */
const std::string menu_item_toggle::get_active_args(void) {
    if(toggled) return arg_on;
    return arg_off;
}

/*
 *
 */
void menu_item_toggle::reset_to_default(void) { toggled = default_val; }

/*
 *
 */
void menu_item_toggle::set_default(void) { toggled = default_val = defaulter(); }

/*
 *
 */
const bool menu_item_toggle::setting_changed(void) const {
    if(toggled == default_val) return false;
    return true;
}

}  // end namespace mnu

}  // end namespace wte
