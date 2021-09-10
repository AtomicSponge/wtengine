/*!
 * WTEngine | File:  selection.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/selection.hpp"

namespace wte::mnu {

/*
 *
 */
selection::selection(
    const std::string& label,
    const std::string& vr,
    const std::vector<std::string>& dvls,
    const std::vector<std::string>& vls
) : item(label), var(vr), display_vals(dvls), vals(vls) {
    assert(dvls.size() == vls.size());
    current_val = vals.begin();
    default_val = current_val;
};

/*
 *
 */
void selection::on_left(void) {
    if(current_val != vals.begin()) current_val--;
};

/*
 *
 */
void selection::on_right(void) {
    if(current_val != vals.end()) current_val++;
    if(current_val == vals.end()) current_val--;
};

/*
 *
 */
const std::vector<std::string> selection::get_text(void) const {
    return {
        get_label(), "< " + display_vals.at(std::distance(vals.cbegin(), current_val)) + " >"
    };
};

/*
 *
 */
void selection::reset_to_default(void) { current_val = default_val; };

/*
 *
 */
void selection::set_default(void) {
    if(current_val == vals.end()) current_val = vals.begin();
    default_val = current_val;
};

/*
 *
 */
const bool selection::setting_changed(void) const {
    if(current_val == default_val) return false;
    return true;
};

/*
 *
 */
const std::string selection::get_setting(void) { return var + "=" + *current_val; };

}  //  end namespace wte::mnu
