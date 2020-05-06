/*!
 * \brief WTEngine | File:  logic.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Logic system.  Override this to define custom ai.
 */

#ifndef WTE_SYS_LOGIC_HPP
#define WTE_SYS_LOGIC_HPP

#include "system.hpp"

namespace wte
{

namespace sys
{

//! Logic system
/*!
  Processes entities that have ai components
*/
class logic : public system {
    public:
        inline logic() : system("logic") {};
        inline ~logic() {};

        //! Run the logic system
        void run(mgr::entity_manager&, mgr::message_manager&, int64_t);    /*!< Run the logic system */
        //! Logic dispatch member
        void dispatch(mgr::entity_manager&, message_container);        /*!< Dispatch logic messages */

    protected:
        component_container ai_components;  /*!< Container for logic components */

        //!  Override this to define run
        virtual void custom_run(mgr::entity_manager&, mgr::message_manager&, int64_t) {};  /*!< Override to define behaviour */
        //!  Override this to process messages sent to the system
        virtual void process_message(mgr::entity_manager&, message) {};  /*!< Override to define behaviour */
};

/*!
  Finds all entities with an ai component and processes their logic
  Override the custom_run member to define behaviour
*/
inline void logic::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    //  Find the entities with the input handler component
    ai_components = world.get_components<cmp::ai>();

    for(component_iterator it = ai_components.begin(); it != ai_components.end(); it++) {
        //
    }

    //  Run custom input handler
    custom_run(world, messages, current_time);
}

/*!
  Get logic messages for processing
  Override the process_messages member to define behaviour
*/
inline void logic::dispatch(mgr::entity_manager& world, message_container messages) {
    component_container dispatch_components = world.get_components<cmp::dispatcher>();

    for(auto it = messages.begin(); it != messages.end(); it++) {
        process_message(world, *it);
    }
}

} //  namespace sys

} //  namespace wte

#endif
