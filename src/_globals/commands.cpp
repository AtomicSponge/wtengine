/*!
 * WTEngine | File:  commands.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/commands.hpp"

namespace wte
{

commands::commands() { _commands.clear(); }
commands::~commands() { _commands.clear(); }

/*
 *
 */
const bool commands::add(const std::string& cmd, const std::function<void(const msg_arg_list&)> func) {
    auto ret = _commands.insert(std::make_pair(cmd, func));
    return ret.second;
}

/*
 *
 */
void commands::process_messages(const message_container& messages) {
    for(auto& it: messages) {
        try { 
            auto res = _commands.find(it.get_cmd());
            if(res != _commands.end()) res->second(it.get_arglist());
        } catch(wte_exception& e) { alert::set(e.what()); }
    }
}

} //  end namespace wte
