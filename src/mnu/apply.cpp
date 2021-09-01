/*!
 * WTEngine | File:  apply.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/apply.hpp"

namespace wte::mnu {

/*
 *
 */
apply::apply() : menu_item("applier"), cmd("cancel") {}

/*
 *
 */
void apply::on_left(void) { cmd = "cancel"; }

/*
 *
 */
void apply::on_right(void) { cmd = "apply"; }

/*
 *
 */
void apply::on_select(void) { mgr::messages::add_message(message("menu", cmd, "")); }

/*
 *
 */
const std::vector<std::string> apply::get_text(void) const {
    if(cmd == "apply") return { "Cancel", "< Apply >" };
    return { "< Cancel >", "Apply" };
}

/*
 *
 */
void apply::reset_to_default(void) { cmd = "cancel"; }

/*
 *
 */
void apply::set_default(void) { cmd = "cancel"; }

}  //  end namespace wte::mnu
