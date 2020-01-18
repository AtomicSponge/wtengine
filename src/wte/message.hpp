/*
  WTEngine
  By:  Matthew Evans
  File:  message.hpp

  See LICENSE.txt for copyright information

  Define the message object
*/

#ifndef WTE_MSG_MESSAGE_HPP
#define WTE_MSG_MESSAGE_HPP

#include <string>
#include <sstream>
#include <vector>

namespace wte
{

namespace msg
{

typedef std::vector<std::string> arg_list;

//! Message class
/*!
  Define individual message objects
*/
class message {
    public:
        inline message() {};
        inline ~message() {};

        inline message(std::string s, std::string c, std::string a) :
                       timer(-1), sys(s), cmd(c), args(a) {};
        inline message(int64_t e, std::string s, std::string c, std::string a) :
                       timer(e), sys(s), cmd(c), args(a) {};
        inline message(std::string s, std::string t, std::string f, std::string c, std::string a) :
                       timer(-1), sys(s), to(t), from(f), cmd(c), args(a) {};
        inline message(int64_t e, std::string s, std::string t, std::string f, std::string c, std::string a) :
                       timer(e), sys(s), to(t), from(f), cmd(c), args(a) {};

        const bool is_timed_event(void) const;
        const arg_list get_split_args(void) const;

        inline const int64_t get_timer(void) const { return timer; };
        inline const std::string get_sys(void) const { return sys; };
        inline const std::string get_to(void) const { return to; };
        inline const std::string get_from(void) const { return from; };
        inline const std::string get_cmd(void) const { return cmd; };
        inline const std::string get_args(void) const { return args; };

        //  Used to sort by timer value
        bool operator<(const message& a) const {
            return timer < a.timer;
        }

    private:
        int64_t timer;          /*!< Timer value that the message will be processed at */
        std::string sys;        /*!< System that will process the message */
        std::string to;         /*!< Message to entity field */
        std::string from;       /*!< Message from entity field */
        std::string cmd;        /*!< Message command */
        std::string args;       /*!< Message arguments */
};

//! Check if the event is synced to the timer
/*!
  -1 means it's not
*/
inline const bool message::is_timed_event(void) const {
    if(timer == -1) return false;
    else return true;
}

//! Split the args up into a vector of strings
/*!
  delimited by ;
*/
inline const arg_list message::get_split_args(void) const {
    std::stringstream arg_stream(args);
    std::string segment;
    std::vector<std::string> arglist;

    while(std::getline(arg_stream, segment, ';')) {
        arglist.push_back(segment);
    }

    return arglist;
}

} //  namespace msg

} //  namespace wte

#endif
