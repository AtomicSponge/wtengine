/*
  WTEngine
  By:  Matthew Evans
  File:  message.hpp

  See LICENSE.txt for copyright information

  Define the message object
  Format:  timer (int64_t), cmd (string), from (string), to (string), args (string)
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

        message(std::string, std::string, std::string, std::string);
        message(int64_t, std::string, std::string, std::string, std::string);

        const bool is_timed_event(void) const;
        const arg_list get_split_args(void) const;

        const int64_t get_timer(void) const;
        const std::string get_from(void) const;
        const std::string get_to(void) const;
        const std::string get_cmd(void) const;
        const std::string get_args(void) const;

        //  Used to sort by timer value
        bool operator<(const message& a) const {
            return timer < a.timer;
        }

    private:
        int64_t timer;          /*!< Timer value that the message will be processed at */
        std::string cmd;        /*!< Message command */
        std::string from;       /*!< Message from field */
        std::string to;         /*!< Message to field */
        std::string args;       /*!< Message arguments */
};

//! Message constructor
/*!
  Create a new message object that is not synced to the timer
*/
inline message::message(std::string c, std::string f, std::string t, std::string a) :
    cmd(c), from(f), to(t), args(a) {
    timer = -1;
}

//! Timed Message constructor
/*!
  Create a new message object that is synced to the timer
*/
inline message::message(int64_t e, std::string c, std::string f, std::string t, std::string a) :
    timer(e), cmd(c), from(f), to(t), args(a) {}

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

//! Message get timer
/*!
  Get timer value of message object
*/
inline const int64_t message::get_timer(void) const { return timer; }

//! Message get command
/*!
  Get command value of message object
*/
inline const std::string message::get_cmd(void) const { return cmd; }

//! Message get from
/*!
  Get from value of message object
*/
inline const std::string message::get_from(void) const { return from; }

//! Message get to
/*!
  Get to value of message object
*/
inline const std::string message::get_to(void) const { return to; }

//! Message get args
/*!
  Get args value of message object
*/
inline const std::string message::get_args(void) const { return args; }

} //  namespace msg

} //  namespace wte

#endif
