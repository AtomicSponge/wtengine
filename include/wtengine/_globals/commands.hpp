/*!
 * WTEngine | File:  commands.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_COMMANDS_HPP
#define WTE_COMMANDS_HPP

#include <string>
#include <map>
#include <functional>

#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

/*!
 * \class commands
 * \brief .
 * 
 * .
 */
class commands {
    public:
        commands();
        ~commands();

        const bool add(const std::string& cmd, const std::function<void(const msg_arg_list&)>& func);
        void process_messages(const message_container& messages);

    private:
        std::map<std::string, std::function<void(const msg_arg_list&)>> _commands;
};

} //  end namespace wte

#endif
