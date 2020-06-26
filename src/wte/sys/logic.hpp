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
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) override {
            //  Find the entities with the input handler component
            component_container<cmp::ai> ai_components = world.set_components<cmp::ai>();

            for(auto & it : ai_components) {
                it.second->run(it.first, world, messages, current_time);
            }
        };
};

} //  namespace sys

} //  namespace wte

#endif
