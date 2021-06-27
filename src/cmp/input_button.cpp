/*!
 * WTEngine | File:  input_button.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/input_button.hpp"

namespace wte
{

namespace cmp
{

input_button::input_button(
    const std::size_t button,
    void func_a(const entity_id&),
    void func_b(const entity_id&)
) : button_flag(button), button_down(func_a), button_up(func_b) {
    assert(button_flag < WTE_MAX_INPUT_BUTTON_FLAGS);
}

const std::size_t input_button::get_flag(void) const {
    return button_flag;
}

void input_button::on_down(const entity_id& e_id) {
    button_down(e_id);
}

void input_button::on_up(const entity_id& e_id) {
    button_up(e_id);
}

} //  namespace cmp

} //  namespace wte
