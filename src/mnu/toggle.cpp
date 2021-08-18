/*!
 * WTEngine | File:  toggle.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/toggle.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
toggle::toggle(
    const std::string label,
    const std::string con,
    const std::string aon,
    const std::string coff,
    const std::string aoff,
    bool func(void)
) : menu_item(label), cmd_on(con), cmd_off(coff), arg_on(aon), arg_off(aoff), defaulter(func) {
    toggled = default_val = defaulter();
}

/*
 *
 */
toggle::toggle(
    const std::string label,
    const std::string con,
    const std::string aon,
    const std::string coff,
    const std::string aoff,
    const std::size_t st,
    bool func(void)
) : menu_item(label, st), cmd_on(con), cmd_off(coff), arg_on(aon), arg_off(aoff), defaulter(func) {
    toggled = default_val = defaulter();
}

/*
 *
 */
toggle::~toggle() {}

/*
 *
 */
void toggle::on_left(void) { toggled = true; }

/*
 *
 */
void toggle::on_right(void) { toggled = false; }

/*
 *
 */
const std::vector<std::string> toggle::get_text(void) const {
    if(toggled) return { get_label(), "< On >", "Off" };
    return { get_label(), "On", "< Off >" };
}

/*
 *
 */
const std::string toggle::get_active_cmd(void) {
    if(toggled) return cmd_on;
    return cmd_off;
}

/*
 *
 */
const std::string toggle::get_active_args(void) {
    if(toggled) return arg_on;
    return arg_off;
}

/*
 *
 */
void toggle::reset_to_default(void) { toggled = default_val; }

/*
 *
 */
void toggle::set_default(void) { toggled = default_val = defaulter(); }

/*
 *
 */
const bool toggle::setting_changed(void) const {
    if(toggled == default_val) return false;
    return true;
}

}  // end namespace mnu

}  // end namespace wte
