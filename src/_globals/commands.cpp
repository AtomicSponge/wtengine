/*!
 * WTEngine | File:  commands.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/commands.hpp"

namespace wte
{

commands::commands() { _commands.clear(); }
commands::~commands() { _commands.clear(); }

const bool commands::add(const std::string& cmd, const std::function<void(const msg_arg_list&)> func) {
    auto ret = _commands.insert(std::make_pair(cmd, func));
    return ret.second;
}

void commands::process_messages(message_container& messages) {
    for(auto& it: messages) try { _commands.find(it.get_cmd())->second(it.get_arglist()); } catch(...) {}
}

} //  end namespace wte
