/*!
 * WTEngine | File:  action.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/action.hpp"

namespace wte::mnu {

/*
 *
 */
action::action(
    const std::string& l,
    const std::string& c,
    const std::string& a
) :
    item(l), cmd(c), arg(a) {};

/*
 *
 */
action::action(
    const std::string& l,
    const std::string& c
) :
    item(l), cmd(c), arg("") {};

/*
 *
 */
void action::on_select(void) { mgr::messages::add(message("system", cmd, arg)); };

/*
 *
 */
const std::vector<std::string> action::get_text(void) const { return { get_label() }; };

}  //  end namespace wte::mnu
