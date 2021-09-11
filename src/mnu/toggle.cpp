/*!
 * WTEngine | File:  toggle.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/toggle.hpp"

namespace wte::mnu {

/*
 *
 */
toggle::toggle(
    const std::string& label,
    const std::function<const bool(void)>& dfunc,
    const std::function<void(void)>& afunc
) : item(label), defaulter(dfunc), applier(afunc) {
    toggled = default_val = defaulter();
}

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
void toggle::reset_to_default(void) { toggled = default_val; }

/*
 *
 */
void toggle::set_default(void) { toggled = default_val = defaulter(); }

/*
 *
 */
void toggle::apply_setting(void) { applier(); }

/*
 *
 */
const bool toggle::setting_changed(void) const {
    if(toggled == default_val) return false;
    return true;
}

}  //  end namespace wte::mnu
