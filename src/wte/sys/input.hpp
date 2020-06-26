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
         * Get all entities tagged with the input_handler component and run
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current value of the main timer.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) override {
            //  Find the entities with the input handler component
            component_container<cmp::input_handler> input_components =
                world.set_components<cmp::input_handler>();

            for(auto & it : input_components) {  //  Loop all input components
                for(auto & i_it : it.second->input_map) {  //  Loop the input map for each component
                    if(input_flags::is_set(i_it.first))  //  Button is down
                        i_it.second.first(it.first, world, messages, current_time);
                    else  //  Button is up
                        i_it.second.second(it.first, world, messages, current_time);
                }  //  End input map loop
            }  //  End input component loop
        };
};

}  //  namespace sys

}  //  namespace wte

#endif
