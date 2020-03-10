/*!
 * \brief WTEngine - File:  message.hpp
 * \author Matthew Evans
 *
 * \copyright See LICENSE.txt for copyright information.
 * \date 2019-2020
 *
 * \details Define the message object.
 */

#ifndef WTE_MSG_MESSAGE_HPP
#define WTE_MSG_MESSAGE_HPP

#include <string>
#include <sstream>
#include <vector>

namespace wte
{

/*!
 * \typedef msg_arg_list Argument strings get split by ; then stored in a vector
 */
typedef std::vector<std::string> msg_arg_list;

//! Message class
/*!
  Define individual message objects
*/
class message {
    public:
        inline message() {};
        inline ~message() {};

        //!  Message Constructor
        /*!
         * Create a non-timed message
         * 
         * \param s System
         * \param c Command
         * \param a Arguments delimited by ;
         */
        inline message(std::string s, std::string c, std::string a) :
            timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); };

        //!  Message Constructor
        /*!
         * Create a timed message
         * 
         * \param e Timer value
         * \param s System
         * \param c Command
         * \param a Arguments delimited by ;
         */
        inline message(int64_t e, std::string s, std::string c, std::string a) :
            timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); };

        //!  Message Constructor
        /*!
         * Create a non-timed message with a to & from
         * 
         * \param s System
         * \param t To
         * \param f From
         * \param c Command
         * \param a Arguments delimited by ;
         */
        inline message(std::string s, std::string t, std::string f, std::string c, std::string a) :
            timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); };

        //!  Message Constructor
        /*!
         * Create a timed message with a to & from
         * 
         * \param e Timer value
         * \param s System
         * \param t To
         * \param f From
         * \param c Command
         * \param a Arguments delimited by ;
         */
        inline message(int64_t e, std::string s, std::string t, std::string f, std::string c, std::string a) :
            timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); };

        //!  Split arguments into a vector of strings.
        /*!
         * \param a String of arguments, delimited by ;
         */
        inline void split_args(const std::string a) {
            if(a == "") arglist.push_back("");
            else {
                std::stringstream arg_stream(a);
                std::string segment;

                while(std::getline(arg_stream, segment, ';')) {
                    arglist.push_back(segment);
                }
            }
        };

        /*!
         * \return The value of timer.
         */
        inline const int64_t get_timer(void) const { return timer; };

        /*!
         * \return The value of sys.
         */
        inline const std::string get_sys(void) const { return sys; };

        /*!
         * \return The value of to.
         */
        inline const std::string get_to(void) const { return to; };

        /*!
         * \return The value of from.
         */
        inline const std::string get_from(void) const { return from; };

        /*!
         * \return The value of cmd.
         */
        inline const std::string get_cmd(void) const { return cmd; };

        /*!
         * \return The number of arguments.
         */
        inline const std::size_t num_args(void) const { return arglist.size(); };

        /*!
         * \return The vector of the arguments.
         */
        inline const msg_arg_list get_arglist(void) const { return arglist; };

        //!  Returns a single argument by index from the argument list
        /*!
         * \param pos The position in the argument vector.
         * \return The argument string by position.
         */
        inline const std::string get_arg(const std::size_t pos) const {
            if(pos > arglist.size()-1) return "";  //  Out of range, return empty string.
            else return arglist[pos];
        };

        //!  Check if the event is synced to the timer.
        /*!
         * \return Returns false if the timer value is -1, else true.
         */
        inline const bool is_timed_event(void) const {
            if(timer == -1) return false;
            else return true;
        };

        //!  Override < operator to sort by timer value.
        /*!
         * \param a Object to compare to.
         */
        bool operator<(const message& a) const {
            return timer < a.timer;
        };

    private:
        int64_t timer;         //  Timer value that the message will be processed at
        std::string sys;       //  System that will process the message
        std::string to;        //  Message to entity field
        std::string from;      //  Message from entity field
        std::string cmd;       //  Message command
        msg_arg_list arglist;  //  Message arguments
};

typedef std::vector<message> message_container;

} //  namespace wte

#endif
