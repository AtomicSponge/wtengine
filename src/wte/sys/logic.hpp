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
         * Logic System constructor.
         * \param void
         * \return void
         */
        inline logic() : system("logic") {};

        /*!
         * Logic System destructor.
         * \param void
         * \return void
         */
        inline ~logic() {};

        /*!
         * \brief Logic system run.
         * Finds all entities with an ai component and processes their logic.
         * entities Reference to entity manager.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t current_time) override {
            //  Find the entities with the input handler component
            component_container<cmp::ai> ai_components = world.set_components<cmp::ai>();

            for(auto & it : ai_components) {
                it.second->run(it.first, world, messages, current_time);
            }
        };

        /*!
         * \brief Get logic messages for processing.
         * Sends messages to each entity with a dispatcher component.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \return void
         */
        inline void dispatch(mgr::entity_manager& world,
                             message_container messages) override {
            //  Get the dispatch componenets.
            component_container<cmp::dispatcher> dispatch_components =
                world.set_components<cmp::dispatcher>();

            for(auto & it : dispatch_components) {
                for(auto m_it = messages.begin(); m_it != messages.end();) {
                    if(m_it->get_to() == world.get_component<cmp::name>(it.first)->name_str) {
                        it.second->run(it.first, world, *m_it);
                        m_it = messages.erase(m_it);
                    } else m_it++;
                    if(messages.empty()) break;  //  Out of messages, end early.
                }
            }
        }
};

} //  namespace sys

} //  namespace wte

#endif
