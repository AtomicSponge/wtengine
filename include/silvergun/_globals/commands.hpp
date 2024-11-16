/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_COMMANDS_HPP)
#define SLV_COMMANDS_HPP

#include <string>
#include <map>
#include <utility>
#include <functional>

#include "silvergun/_globals/message.hpp"

namespace slv {

/*!
 * \class commands
 * \brief Container for storing engine commands expressed as a lambda.
 */
class commands final {
  private:
    //  Container for commands.
    std::map<std::string, std::pair<std::size_t, std::function<void(const msg_args&)>>> _commands;

  public:
    commands() = default;   //  Default constructor.
    ~commands() = default;  //  Default destructor.

    /*!
     * \brief Add a command to the command processor.
     * \param cmd Command name to run.
     * \param nargs Minimum number of expected arguments.
     * \param func Lambda expression to run.
     * \return True on sucess, false on fail.
     */
    bool add(
      const std::string& cmd,
      const std::size_t& nargs,
      const std::function<void(const msg_args&)>& func
    ) {
      auto ret = _commands.insert(std::make_pair(cmd, std::make_pair(nargs, func)));
      return ret.second;
    };

    /*!
     * \brief Process a list of messages.
     * \param messages List of messages to process.
     */
    void process_messages(const message_container& messages) {
      for (auto& it: messages) {
        auto res = _commands.find(it.get_cmd());
        //  Check to make sure there are enough arguments to run the command.
        if (res != _commands.end() && it.num_args() >= res->second.first)
          res->second.second(it.get_args());
      }
    };
};

}

#endif
