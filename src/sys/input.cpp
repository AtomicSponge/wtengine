/*!
 * WTEngine | File:  input.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/input.hpp"

namespace wte
{

namespace sys
{

/*
 *
 */
input::input() : system("input") {}

/*
 *
 */
input::~input() {}

/*
 *
 */
void input::disable(void) { enabled = false; }

/*
 *
 */
void input::run(void) {
    //  First check all directional components.
    component_container<cmp::input_directional> directional_components =
        mgr::entities::set_components<cmp::input_directional>();

    for(auto & d_it : directional_components) {
        if(input_flags::joystick_check(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET)) {
            d_it.second->on_set(d_it.first,
                        input_flags::get_joystick_radians(d_it.second->get_bind()));
        }
        if(input_flags::joystick_check(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET)) {
            d_it.second->on_unset(d_it.first);
        }
    }

    //  Now check all button components.
    component_container<cmp::input_button> button_components =
        mgr::entities::set_components<cmp::input_button>();

    for(auto & b_it : button_components) {
        if(input_flags::check_button_event(b_it.second->get_flag(), WTE_BUTTON_EVENT_DOWN))
            b_it.second->on_down(b_it.first);
        if(input_flags::check_button_event(b_it.second->get_flag(), WTE_BUTTON_EVENT_UP))
            b_it.second->on_up(b_it.first);

    }  //  End input component loop
}

}  //  namespace sys

}  //  namespace wte
