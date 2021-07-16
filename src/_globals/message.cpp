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
message::message(std::string s, std::string c, std::string a) :
timer(-1), sys(s), to(""), from(""), cmd(c) { split_args(a); };

/*
 *
 */
message::message(int64_t e, std::string s, std::string c, std::string a) :
timer(e), sys(s), to(""), from(""), cmd(c) { split_args(a); };

/*
 *
 */
message::message(std::string s, std::string t, std::string f, std::string c, std::string a) :
timer(-1), sys(s), to(t), from(f), cmd(c) { split_args(a); };

/*
 *
 */
message::message(int64_t e, std::string s, std::string t, std::string f, std::string c, std::string a) :
timer(e), sys(s), to(t), from(f), cmd(c) { split_args(a); };

}  //  end namespace wte
