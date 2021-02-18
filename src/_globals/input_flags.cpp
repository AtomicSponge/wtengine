/*!
 * WTEngine | File:  input_flags.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <cassert>

#include "wtengine/_globals/input_flags.hpp"

namespace wte
{

void input_flags::unset_all(void) {
    for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
        for(std::size_t d = 0; d < WTE_MAX_DIRECTON_FLAGS; d++)
            dflags[j][d] = false;

    for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
        angle[j] = 0.0f;
    for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
        pol_x[j] = 0.0f;
    for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
        pol_y[j] = 0.0f;

    for(std::size_t b = 0; b < WTE_MAX_INPUT_BUTTON_FLAGS; b++)
        for(std::size_t e = 0; e < WTE_MAX_BUTTON_EVENT_FLAGS; e++)
            buttons[b][e] = false;
}

void input_flags::joystick_toggle(const std::size_t& j, const std::size_t& d) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);
    assert(d < WTE_MAX_DIRECTON_FLAGS);

    dflags[j][d] = true;
}

const bool input_flags::joystick_check(const std::size_t& j, const std::size_t& d) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);
    assert(d < WTE_MAX_DIRECTON_FLAGS);
    
    const bool consume = dflags[j][d];
    if(consume) dflags[j][d] = false;
    return consume;
}

void input_flags::set_joystick_radians(const std::size_t& j, const float& a) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    angle[j] = a;
}

const float input_flags::get_joystick_radians(const std::size_t& j) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    return angle[j];
}

void input_flags::set_joystick_pol_x(const std::size_t& j, const float& d) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    pol_x[j] = d;
}

void input_flags::set_joystick_pol_y(const std::size_t& j, const float& d) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    pol_y[j] = d;
}

const float input_flags::get_joystick_pol_x(const std::size_t& j) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    return pol_x[j];
}

const float input_flags::get_joystick_pol_y(const std::size_t& j) {
    assert(j < WTE_MAX_JOYSTICK_FLAGS);

    return pol_y[j];
}

const bool input_flags::check_button_event(const std::size_t& b, const std::size_t& e) {
    assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
    assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

    const bool consume = buttons[b][e];
    if(consume) buttons[b][e] = false;
    return consume;
}

void input_flags::set_button_event(const std::size_t& b, const std::size_t& e) {
    assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
    assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

    buttons[b][e] = true;
}

const std::size_t input_flags::get_last_keypress(void) {
    return last_keypress;
}

void input_flags::set_last_keypress(const std::size_t& lk) {
    last_keypress = lk;
}

const std::size_t input_flags::get_last_buttonpress(void) {
    return last_buttonpress;
}

void input_flags::set_last_buttonpress(const std::size_t& lk) {
    last_buttonpress = lk;
}

} //  end namespace wte
