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
apply::apply() : menu_item("Apply"), do_apply(false) {}

/*
 *
 */
void apply::on_left(void) { do_apply = false; }

/*
 *
 */
void apply::on_right(void) { do_apply = true; }

/*
 *
 */
void apply::on_select(void) {}

/*
 *
 */
const std::vector<std::string> apply::get_text(void) const {
    if(do_apply) return { "Cancel", "< Apply >" };
    return { "< Cancel >", "Apply" };
}

/*
 *
 */
void apply::reset_to_default(void) { do_apply = false; }

/*
 *
 */
void apply::set_default(void) { do_apply = false; }

}  //  end namespace wte::mnu
