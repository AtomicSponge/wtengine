/*!
 * WTEngine | File:  dispatcher.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <functional>

#include "wtengine/cmp/dispatcher.hpp"
#include "wtengine/mgr/message_manager.hpp"
#include "wtengine/mgr/entity_manager.hpp"
#include "wtengine/message.hpp"

namespace wte
{

namespace cmp
{

dispatcher::dispatcher(void func(const entity_id&,
                     const message&,
                     mgr::entity_manager&,
                     mgr::message_manager&,
                     const int64_t&)) : handle_msg(func) {};

void dispatcher::proc_msg(const entity_id& e_id,
              const message& msg,
              mgr::entity_manager& world,
              mgr::message_manager& messages,
              const int64_t& current_time) {
    handle_msg(e_id, msg, world, messages, current_time);
}

} //  namespace cmp

} //  namespace wte
