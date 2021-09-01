/*!
 * WTEngine | File:  dispatcher.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/dispatcher.hpp"

namespace wte::cmp {

/*
 *
 */
dispatcher::dispatcher(const std::function<void(const entity_id&, const message&)>& func) : handle_msg(func) {}

/*
 *
 */
void dispatcher::proc_msg(const entity_id& e_id, const message& msg) { try { handle_msg(e_id, msg); } catch(...) { throw; } }

}  //  end namespace wte::cmp
