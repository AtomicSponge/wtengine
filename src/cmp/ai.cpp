/*!
 * WTEngine | File:  ai.cpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include <functional>

#include "wtengine/cmp/ai.hpp"

namespace wte
{

namespace cmp
{

ai::ai(void func(const entity_id&,
                 mgr::entity_manager&,
                 mgr::message_manager&,
                 const int64_t&)) :
    enabled_ai(func), disabled_ai([](const entity_id& e_id,
                                     mgr::entity_manager& world,
                                     mgr::message_manager& messages,
                                     const int64_t& engine_time){}) {}

ai::ai(void func_a(const entity_id&,
                   mgr::entity_manager&,
                   mgr::message_manager&,
                   const int64_t&),
       void func_b(const entity_id&,
                   mgr::entity_manager&,
                   mgr::message_manager&,
                   const int64_t&)) :
    enabled_ai(func_a), disabled_ai(func_b) {}

void ai::run_enabled(const entity_id& eid,
                     mgr::entity_manager& world,
                     mgr::message_manager& messages,
                     const int64_t& engine_time) {
    enabled_ai(eid, world, messages, engine_time);
}

void ai::run_disabled(const entity_id& eid,
                      mgr::entity_manager& world,
                      mgr::message_manager& messages,
                      const int64_t& engine_time) {
    disabled_ai(eid, world, messages, engine_time);
}

} //  namespace cmp

} //  namespace wte
