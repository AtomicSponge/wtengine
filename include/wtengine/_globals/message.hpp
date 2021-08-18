/*!
 * WTEngine | File:  message.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MSG_MESSAGE_HPP
#define WTE_MSG_MESSAGE_HPP

#include <string>
#include <vector>
#include <sstream>

namespace wte {

/*!
 * \typedef std::vector<std::string> msg_args
 * Container to store each message argument separately.
 */
typedef std::vector<std::string> msg_args;

/*!
 * \class message
 * \brief Define individual message objects.
 */
class message final {
    public:
        /*!
         * \brief Create a non-timed message.
         * \param s System.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        message(
            const std::string& s,
            const std::string& c,
            const std::string& a
        );

        /*!
         * \brief Create a timed message.
         * \param e Timer value.
         * \param s System.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        message(
            const int64_t& e,
            const std::string& s,
            const std::string& c,
            const std::string& a
        );

        /*!
         * \brief Create a non-timed message with a to & from.
         * \param s System.
         * \param t To.
         * \param f From.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        message(
            const std::string& s,
            const std::string& t,
            const std::string& f,
            const std::string& c,
            const std::string& a
        );

        /*!
         * \brief Create a timed message with a to & from.
         * \param e Timer value.
         * \param s System.
         * \param t To.
         * \param f From.
         * \param c Command.
         * \param a Arguments delimited by ;
         */
        message(
            const int64_t& e,
            const std::string& s,
            const std::string& t,
            const std::string& f,
            const std::string& c,
            const std::string& a
        );

        /*!
         * \brief Overload < operator to sort by timer value.
         * \param m Object to compare to.
         * \return True if less than, false if greater than.
         */
        const bool operator<(
            const message& m
        ) const;

        /*!
         * \brief Split arguments into a vector of strings.
         * \param a String of arguments, delimited by ;
         */
        void split_args(
            const std::string& a
        );

        /*!
         * \brief Get timer value.
         * \return The value of timer.
         */
        const int64_t get_timer(void) const;

        /*!
         * \brief Get system value.
         * \return The value of sys.
         */
        const std::string get_sys(void) const;

        /*!
         * \brief Get to value.
         * \return The value of to.
         */
        const std::string get_to(void) const;

        /*!
         * \brief Get from value.
         * \return The value of from.
         */
        const std::string get_from(void) const;

        /*!
         * \brief Get command value.
         * \return The value of cmd.
         */
        const std::string get_cmd(void) const;

        /*!
         * \brief Get number of arguments.
         * \return The number of arguments.
         */
        const std::size_t num_args(void) const;

        /*!
         * \brief Get the arguments split into a vector.
         * \return The vector of the arguments.
         */
        const msg_args get_args(void) const;

        /*!
         * \brief Returns a single argument by pos from the argument list.
         * \param pos The position in the argument vector.
         * \return The argument string by position.
         */
        const std::string get_arg(
            const std::size_t& pos
        ) const;

        /*!
         * \brief Check if the event is synced to the timer.
         * \return Returns false if the timer value is -1, else true.
         */
        const bool is_timed_event(void) const;

    private:
        int64_t timer;      //  Timer value that the message will be processed at
        std::string sys;    //  System that will process the message
        std::string to;     //  Message to entity field
        std::string from;   //  Message from entity field
        std::string cmd;    //  Message command
        msg_args args;      //  Message arguments
};

/*!
 * \typedef std::vector<message> message_container
 * Container to store a collection of messages.
 */
typedef std::vector<message> message_container;

} //  namespace wte

#endif
