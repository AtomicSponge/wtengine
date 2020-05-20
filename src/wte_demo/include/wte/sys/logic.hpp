/*!
 * \brief WTEngine | File:  logic.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Logic system.
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
 * Processes entities that have ai components.
 * Also sends messages to entities with dispatch components.
 */
class logic final : public system {
    public:
        /*!
         * ...
         */
        inline logic() : system("logic") {};

        /*!
         * ...
         */
        inline ~logic() {};

        /*!
         * Finds all entities with an ai component and processes their logic
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t current_time) {
            //  Find the entities with the input handler component
            component_container ai_components = world.get_components<cmp::ai>();

            for(component_iterator it = ai_components.begin(); it != ai_components.end(); it++) {
                dynamic_cast<cmp::ai*>(it->second.get())->run(it->first, world, messages, current_time);
            }
        }

        /*!
         * Get logic messages for processing
         */
        inline void dispatch(mgr::entity_manager& world, message_container messages) {
            component_container dispatch_components = world.get_components<cmp::dispatcher>();

            for(component_iterator it = dispatch_components.begin(); it != dispatch_components.end(); it++) {
                for(auto m_it = messages.begin(); m_it != messages.end();) {
                    if(m_it->get_to() == world.get_component<cmp::name>(it->first)->name_str) {
                        dynamic_cast<cmp::dispatcher*>(it->second.get())->run(it->first, world, *m_it);
                        m_it = messages.erase(m_it);
                    } else m_it++;
                }
                if(messages.empty()) break;  //  Out of messages, end early.
            }
        }
};

} //  namespace sys

} //  namespace wte

#endif
