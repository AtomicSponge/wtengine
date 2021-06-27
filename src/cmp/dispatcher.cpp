/*!
 * WTEngine | File:  dispatcher.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/dispatcher.hpp"

namespace wte
{

namespace cmp
{

dispatcher::dispatcher(
    void func(const entity_id&, const message&)
) : handle_msg(func) {};

void dispatcher::proc_msg(const entity_id& e_id, const message& msg) {
    handle_msg(e_id, msg);
}

} //  namespace cmp

} //  namespace wte
