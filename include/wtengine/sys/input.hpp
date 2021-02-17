/*!
 * WTEngine | File:  input.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_SYS_INPUT_HPP
#define WTE_SYS_INPUT_HPP

#include "wtengine/system.hpp"
#include "wtengine/_globals/input_flags.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class input
 * \brief Processes keyboard input
 */
class input final : public system {
    public:
        input();
        ~input();

        void disable(void) override;

        /*!
         * \brief Input system run method
         * 
         * Get all entities tagged with an input handler component and run.
         * 
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current value of the main timer.
         */
        void run(mgr::entity_manager& world,
                 mgr::message_manager& messages,
                 const int64_t& current_time) override;
};

}  //  namespace sys

}  //  namespace wte

#endif
