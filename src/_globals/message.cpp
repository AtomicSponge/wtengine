/*!
 * wtengine | File:  message.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/_globals/message.hpp"

namespace wte {

/*
 *
 */
message::message(const std::string& s, const std::string& c, const std::string& a) :
timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); }

/*
 *
 */
message::message(const int64_t& e, const std::string& s, const std::string& c, const std::string& a) :
timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); }

/*
 *
 */
message::message(const std::string& s, const std::string& t, const std::string& f, const std::string& c, const std::string& a) :
timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); }

/*
 *
 */
message::message(const int64_t& e, const std::string& s, const std::string& t, const std::string& f, const std::string& c, const std::string& a) :
timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); }

/*
 *
 */
bool message::operator<(const message& m) const { return timer < m.timer; }

/*
 *
 */
void message::split_args(const std::string& a) {
    if(a == "") args.push_back("");
    else {
        std::stringstream arg_stream(a);
        std::string segment;

        while(std::getline(arg_stream, segment, ';')) {
            args.push_back(segment);
        }
    }
}

/*
 *
 */
int64_t message::get_timer(void) const { return timer; }

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
std::size_t message::num_args(void) const { return args.size(); }

/*
 *
 */
const msg_args message::get_args(void) const { return args; }

/*
 *
 */
const std::string message::get_arg(const std::size_t& pos) const {
    if(pos >= args.size()) return "";  //  Out of range, return empty string.
    else return args[pos];
}

/*
 *
 */
bool message::is_timed_event(void) const {
    if(timer == -1) return false;
    else return true;
}

}  //  end namespace wte
