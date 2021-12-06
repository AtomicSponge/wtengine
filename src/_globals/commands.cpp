/*!
 * WTEngine | File:  commands.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/commands.hpp"

namespace wte {

/*
 *
 */
const bool commands::add(
    const std::string& cmd,
    const std::size_t& nargs,
    const std::function<void(const msg_args&)>& func
) {
    auto ret = _commands.insert(std::make_pair(cmd, std::make_pair(nargs, func)));
    return ret.second;
}

/*
 *
 */
void commands::process_messages(
    const message_container& messages
) {
    for(auto& it: messages) {
        auto res = _commands.find(it.get_cmd());
        //  Check to make sure there are enough arguments to run the command.
        if(res != _commands.end() && it.num_args() >= res->second.first)
            res->second.second(it.get_args());
    }
}

} //  end namespace wte
