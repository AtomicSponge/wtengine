/*!
 * WTEngine | File:  input_directional.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/input_directional.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
input_directional::input_directional(
    const std::size_t ib,
    std::function<void(const entity_id&, const float&)> func_a,
    std::function<void(const entity_id&)> func_b
) : input_bind(ib), direction_set(func_a), direction_unset(func_b) {
    assert(input_bind < WTE_MAX_JOYSTICK_FLAGS);
}

/*
 *
 */
const std::size_t input_directional::get_bind(void) const { return input_bind; }

/*
 *
 */
void input_directional::on_set(const entity_id& e_id, const float& rad) {
    try { direction_set(e_id, rad); } catch(const wte_exception& e) { alert::set_alert(e.what()); }
}

/*
 *
 */
void input_directional::on_unset(const entity_id& e_id) {
    try { direction_unset(e_id); } catch(const wte_exception& e) { alert::set_alert(e.what()); }
}

} //  namespace cmp

} //  namespace wte
