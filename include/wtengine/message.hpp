/*!
 * WTEngine | File:  message.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MSG_MESSAGE_HPP
#define WTE_MSG_MESSAGE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <any>

namespace wte
{

/*!
 * \typedef std::vector<std::string> msg_arg_list
 * Container to store each message argument separately.
 */
typedef std::vector<std::string> msg_arg_list;

/*!
 * \class message
 * \brief Define individual message objects
 */
class message {
    public:
        inline message() {};
        inline ~message() {};

        /*!
         * \brief Message Constructor
         * 
         * Create a non-timed message.
         * 
         * \param s System.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        inline message(std::string s, std::string c, std::string a) :
            timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); };

        /*!
         * \brief Message Constructor
         * 
         * Create a timed message.
         * 
         * \param e Timer value.
         * \param s System.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        inline message(int64_t e, std::string s, std::string c, std::string a) :
            timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); };

        /*!
         * \brief Message Constructor
         * 
         * Create a non-timed message with a to & from.
         * 
         * \param s System.
         * \param t To.
         * \param f From.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        inline message(std::string s, std::string t, std::string f, std::string c, std::string a) :
            timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); };

        /*!
         * \brief Message Constructor
         * 
         * Create a timed message with a to & from.
         * 
         * \param e Timer value.
         * \param s System.
         * \param t To.
         * \param f From.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        inline message(int64_t e, std::string s, std::string t, std::string f, std::string c, std::string a) :
            timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); };

        /*!
         * \brief Overload < operator to sort by timer value.
         * 
         * \param a Object to compare to.
         * \return True if less than, false if greater than.
         */
        inline const bool operator<(const message& a) const {
            return timer < a.timer;
        };

        /*!
         * \brief Split arguments into a vector of strings.
         * 
         * \param a String of arguments, delimited by ;
         */
        inline void split_args(const std::string& a) {
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
         * \brief Get timer value.
         * 
         * \return The value of timer.
         */
        inline const int64_t get_timer(void) const { return timer; };

        /*!
         * \brief Get system value.
         * 
         * \return The value of sys.
         */
        inline const std::string get_sys(void) const { return sys; };

        /*!
         * \brief Get to value.
         * 
         * \return The value of to.
         */
        inline const std::string get_to(void) const { return to; };

        /*!
         * \brief Get from value.
         * 
         * \return The value of from.
         */
        inline const std::string get_from(void) const { return from; };

        /*!
         * \brief Get command value.
         * 
         * \return The value of cmd.
         */
        inline const std::string get_cmd(void) const { return cmd; };

        /*!
         * \brief Get number of arguments.
         * 
         * \return The number of arguments.
         */
        inline const std::size_t num_args(void) const { return arglist.size(); };

        /*!
         * \brief Get the arguments split into a vector.
         * 
         * \return The vector of the arguments.
         */
        inline const msg_arg_list get_arglist(void) const { return arglist; };

        /*!
         * \brief Returns a single argument by pos from the argument list.
         * 
         * \param pos The position in the argument vector.
         * \return The argument string by position.
         */
        inline const std::string get_arg(const std::size_t& pos) const {
            if(pos >= arglist.size()) return "";  //  Out of range, return empty string.
            else return arglist[pos];
        };

        /*!
         * \brief Get a single argument by pos from the argument list, casted to a type.
         * 
         * \tparam T Type to cast to.
         * \param pos The position in the argument vector.
         * \return The argument by position casted to type.
         */
        template <typename T>
        inline const T get_arg(const std::size_t& pos) const {
            //  Bad position, return false.
            if(pos >= arglist.size()) return std::any_cast<const bool>(false);
            try {
                T temp;
                std::stringstream(arglist[pos]) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                return std::any_cast<const T>(temp);
            } catch (std::bad_any_cast& e) {
                return std::any_cast<const bool>(false);  //  Bad cast, return false.
            }
        };

        /*!
         * \brief Check if the event is synced to the timer.
         * 
         * \return Returns false if the timer value is -1, else true.
         */
        inline const bool is_timed_event(void) const {
            if(timer == -1) return false;
            else return true;
        };

    private:
        int64_t timer;         //  Timer value that the message will be processed at
        std::string sys;       //  System that will process the message
        std::string to;        //  Message to entity field
        std::string from;      //  Message from entity field
        std::string cmd;       //  Message command
        msg_arg_list arglist;  //  Message arguments
};

/*!
 * \typedef std::vector<message> message_container
 * Container to store a collection of messages.
 */
typedef std::vector<message> message_container;

} //  namespace wte

#endif
