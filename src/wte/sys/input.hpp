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
#include "../cmp/input_handler.hpp"
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
         * Get all entities tagged with the input_handler component and run.
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
                if(input_flags::check(WTE_INPUT_DIRECTON_SET)) {
                    d_it.second->on_set(d_it.first, world, messages, current_time);
                }
                if(input_flags::check(WTE_INPUT_DIRECTION_UNSET)) {
                    d_it.second->on_unset(d_it.first, world, messages, current_time);
                }
            }

            //  Find the entities with the input handler component
            component_container<cmp::input_handler> input_components =
                world.set_components<cmp::input_handler>();

            for(auto & b_it : input_components) {  //  Loop all input components
                for(auto & i_it : b_it.second->get_map()) {  //  Loop the input map
                    if(input_flags::check(i_it.first))  //  Check if there is an event to consume.
                        i_it.second(b_it.first, world, messages, current_time);
                }
            }  //  End input component loop
        };
};

}  //  namespace sys

}  //  namespace wte

#endif
