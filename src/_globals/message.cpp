/*!
 * WTEngine | File:  message.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/message.hpp"

namespace wte
{

/*
 *
 */
message::message(const std::string& s, const std::string& c, const std::string& a) :
timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); };

/*
 *
 */
message::message(const int64_t& e, const std::string& s, const std::string& c, const std::string& a) :
timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); };

/*
 *
 */
message::message(const std::string& s, const std::string& t, const std::string& f, const std::string& c, const std::string& a) :
timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); };

/*
 *
 */
message::message(const int64_t& e, const std::string& s, const std::string& t, const std::string& f, const std::string& c, const std::string& a) :
timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); };

/*
 *
 */
const bool message::operator<(const message& m) const { return timer < m.timer; }

/*
 *
 */
void message::split_args(const std::string& a) {
    if(a == "") arglist.push_back("");
    else {
        std::stringstream arg_stream(a);
        std::string segment;

        while(std::getline(arg_stream, segment, ';')) {
            arglist.push_back(segment);
        }
    }
}

/*
 *
 */
const int64_t message::get_timer(void) const { return timer; }

/*
 *
 */
const std::string message::get_sys(void) const { return sys; }

/*
 *
 */
const std::string message::get_to(void) const { return to; }

/*
 *
 */
const std::string message::get_from(void) const { return from; }

/*
 *
 */
const std::string message::get_cmd(void) const { return cmd; }

/*
 *
 */
const std::size_t message::num_args(void) const { return arglist.size(); }

/*
 *
 */
const msg_arg_list message::get_arglist(void) const { return arglist; }

/*
 *
 */
const std::string message::get_arg(const std::size_t& pos) const {
    if(pos >= arglist.size()) return "";  //  Out of range, return empty string.
    else return arglist[pos];
}

/*
 *
 */
const bool message::is_timed_event(void) const {
    if(timer == -1) return false;
    else return true;
}

}  //  end namespace wte
