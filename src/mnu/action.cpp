/*!
 * WTEngine | File:  action.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/action.hpp"

namespace wte
{

namespace mnu
{

/*
 *
 */
action::action(const std::string l, const std::string c, const std::string a) :
    menu_item(l), cmd(c), arg(a) {};

/*
 *
 */
action::action(const std::string l, const std::string c) :
    menu_item(l), cmd(c), arg("") {};

/*
 *
 */
action::~action() {};

/*
 *
 */
const message action::on_select(void) { return message("system", cmd, arg); };

/*
 *
 */
const std::vector<std::string> action::get_text(void) const { return { get_label() }; };

}  // end namespace mnu

}  // end namespace wte
