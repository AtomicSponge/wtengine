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

namespace wte
{

namespace sys
{

//! Input system
/*!
  Processes keyboard input
*/
class input : public system {
    public:
        inline input() : system("input") {};
        inline ~input() {};

        inline void disable(void) { enabled = false; };

        //! Input system run method
        /*!
         * Get all entities tagged with the input_handler component and store for processing
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t current_time) {
            //  Find the entities with the input handler component
            input_components = world.set_components<cmp::input_handler>();

            //  Run custom input handler
            custom_run(world, messages);
        }

    protected:
        component_container input_components;
        //!  Override this to implement input handling
        virtual void custom_run(mgr::entity_manager&, mgr::message_manager&) {};
};

} //  namespace sys

} //  namespace wte

#endif
