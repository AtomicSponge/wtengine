/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MESSAGE_HPP)
#define WTE_MESSAGE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <cstdint>

namespace wte {

/*!
 * \typedef std::vector<std::string> msg_args
 * Container to store each message argument separately.
 */
using msg_args = std::vector<std::string>;

/*!
 * \class message
 * \brief Define individual message objects.
 */
class message final {
  private:
    //  Split arguments into a vector of strings.
    void split_args(const std::string& a) {
      if (a == "") args.push_back("");
      else {
        std::stringstream arg_stream(a);
        std::string segment;

        while (std::getline(arg_stream, segment, ';')) {
          args.push_back(segment);
        }
      }
    };

    int64_t timer;      //  Timer value that the message will be processed at
    std::string sys;    //  System that will process the message
    std::string to;     //  Message to entity field
    std::string from;   //  Message from entity field
    std::string cmd;    //  Message command
    msg_args args;      //  Message arguments

  public:
    message() = delete;  //  Delete default constructor.

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
    ) : timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); };

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
    ) : timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); };

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
    ) : timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); };

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
    ) : timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); };

    /*
     * Overload < operator to sort by timer value.
     */
    bool operator<(const message& m) const { return timer < m.timer; };

    /*!
     * \brief Get timer value.
     * \return The value of timer.
     */
    int64_t get_timer(void) const { return timer; };

    /*!
     * \brief Get system value.
     * \return The value of sys.
     */
    const std::string get_sys(void) const { return sys; };

    /*!
     * \brief Get to value.
     * \return The value of to.
     */
    const std::string get_to(void) const { return to; };

    /*!
     * \brief Get from value.
     * \return The value of from.
     */
    const std::string get_from(void) const { return from; };

    /*!
     * \brief Get command value.
     * \return The value of cmd.
     */
    const std::string get_cmd(void) const { return cmd; };

    /*!
     * \brief Get number of arguments.
     * \return The number of arguments.
     */
    std::size_t num_args(void) const { return args.size(); };

    /*!
     * \brief Get the arguments split into a vector.
     * \return The vector of the arguments.
     */
    const msg_args get_args(void) const { return args; };

    /*!
     * \brief Returns a single argument by pos from the argument list.
     * \param pos The position in the argument vector.
     * \return The argument string by position.
     */
    const std::string get_arg(const std::size_t& pos) const {
      if (pos >= args.size()) return "";  //  Out of range, return empty string.
      else return args[pos];
    };

    /*!
     * \brief Check if the event is synced to the timer.
     * \return Returns false if the timer value is -1, else true.
     */
    bool is_timed_event(void) const {
      if (timer == -1) return false;
      else return true;
    };
};

/*!
 * \typedef std::vector<message> message_container
 * Container to store a collection of messages.
 */
typedef std::vector<message> message_container;

}  //  end namespace wte

#endif
