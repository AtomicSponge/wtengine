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
 * \brief Container for storing engine commands expressed as a lambda.
 */
class commands {
    public:
        commands();
        ~commands();

        /*!
         * \brief Add a command to the command processor.
         *
         * \param cmd Command name to run.
         * \param func Lambda expression to run.
         * \return True on sucess, false on fail.
         */
        const bool add(const std::string& cmd, const std::function<void(const msg_arg_list&)>& func);

        /*!
         * \brief Process a list of messages.
         *
         * \param messages List of messages to process.
         */
        void process_messages(const message_container& messages);

    private:
        //  Container for commands.
        std::map<std::string, std::function<void(const msg_arg_list&)>> _commands;
};

} //  end namespace wte

#endif
