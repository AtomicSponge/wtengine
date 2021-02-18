/*!
 * WTEngine | File:  input_directional.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <functional>
#include <cassert>

#include "wtengine/component.hpp"
#include "wtengine/mgr/entity_manager.hpp"
#include "wtengine/mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

input_directional(
    const std::size_t ib,
    void func_a(const entity_id&,
                const float&,
                mgr::entity_manager&,
                mgr::message_manager&,
                const int64_t&),
    void func_b(const entity_id&,
                mgr::entity_manager&,
                mgr::message_manager&,
                const int64_t&)
) : input_bind(ib), direction_set(func_a), direction_unset(func_b) {
    assert(input_bind < WTE_MAX_JOYSTICK_FLAGS);
}

const std::size_t get_bind(void) const {
    return input_bind;
}

void on_set(const entity_id& e_id,
            const float& rad,
            mgr::entity_manager& world,
            mgr::message_manager& messages,
            const int64_t& current_time) {
    direction_set(e_id, rad, world, messages, current_time);
}

void on_unset(const entity_id& e_id,
              mgr::entity_manager& world,
              mgr::message_manager& messages,
              const int64_t& current_time) {
    direction_unset(e_id, world, messages, current_time);
}

} //  namespace cmp

} //  namespace wte
