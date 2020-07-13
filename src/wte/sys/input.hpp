/*!
 * \brief WTEngine | File:  input.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input system.  Override this to create a custom input system.
 */

#ifndef WTE_SYS_INPUT_HPP
#define WTE_SYS_INPUT_HPP

#include "system.hpp"
#include "../_globals/input_flags.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class Input system
 * Processes keyboard input
 */
class input final : public system {
    public:
        inline input() : system("input") {};
        inline ~input() {};

        inline void disable(void) override { enabled = false; };

        /*!
         * Input system run method
         * Get all entities tagged with an input handler component and run.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current value of the main timer.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) override {
            //  First check all directional components.
            component_container<cmp::input_directional> directional_components =
                world.set_components<cmp::input_directional>();

            for(auto & d_it : directional_components) {
                if(input_flags::joystick_check(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_SET)) {
                    d_it.second->on_set(d_it.first,
                                input_flags::get_joystick_radians(d_it.second->get_bind()),
                                world, messages, current_time);
                }
                if(input_flags::joystick_check(WTE_JOYSTICK_A, WTE_INPUT_DIRECTION_UNSET)) {
                    d_it.second->on_unset(d_it.first, world, messages, current_time);
                }
            }

            //  Now check all button components.
            component_container<cmp::input_button> button_components =
                world.set_components<cmp::input_button>();

            for(auto & b_it : button_components) {
                if(input_flags::check_button_event(b_it.second->get_flag(), WTE_BUTTON_EVENT_DOWN))
                    b_it.second->on_down(b_it.first, world, messages, current_time);
                if(input_flags::check_button_event(b_it.second->get_flag(), WTE_BUTTON_EVENT_UP))
                    b_it.second->on_up(b_it.first, world, messages, current_time);

            }  //  End input component loop
        };
};

}  //  namespace sys

}  //  namespace wte

#endif
