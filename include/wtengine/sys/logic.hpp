/*!
 * WTEngine | File:  logic.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_LOGIC_HPP
#define WTE_SYS_LOGIC_HPP

#include "wtengine/sys/system.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class logic
 * \brief Processes entities that have ai components.
 * 
 * Also sends messages to entities with dispatch components.
 */
class logic final : public system {
    public:
        /*!
         * \brief Logic System constructor.
         */
        logic();

        /*!
         * \brief Logic System destructor.
         */
        ~logic();

        /*!
         * \brief Logic system run.
         * 
         * Finds all entities with an ai component and processes their logic.
         * 
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void run(mgr::entity_manager& world,
                 mgr::message_manager& messages,
                 const int64_t& current_time) override;
};

} //  namespace sys

} //  namespace wte

#endif
