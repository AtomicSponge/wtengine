/*!
 * WTEngine | File:  input_button.hpp
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

input_button(
    const std::size_t button,
    void func_a(const entity_id&,
                mgr::entity_manager&,
                mgr::message_manager&,
                const int64_t&),
    void func_b(const entity_id&,
                mgr::entity_manager&,
                mgr::message_manager&,
                const int64_t&)
) : button_flag(button), button_down(func_a), button_up(func_b) {
    assert(button_flag < WTE_MAX_INPUT_BUTTON_FLAGS);
}

const std::size_t get_flag(void) const {
    return button_flag;
}

void on_down(const entity_id& e_id,
             mgr::entity_manager& world,
             mgr::message_manager& messages,
             const int64_t& current_time) {
    button_down(e_id, world, messages, current_time);
}

void on_up(const entity_id& e_id,
           mgr::entity_manager& world,
           mgr::message_manager& messages,
           const int64_t& current_time) {
    button_up(e_id, world, messages, current_time);
}

} //  namespace cmp

} //  namespace wte

#endif
