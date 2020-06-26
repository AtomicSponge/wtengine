/*!
 * \brief WTEngine | File:  ai.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details AI component - tag to be processed by logic system.
 */

#ifndef WTE_CMP_AI_HPP
#define WTE_CMP_AI_HPP

#include <functional>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

//! AI component
/*!
 * Tag components to be processed by the Logic system.
 * Define logic in do_ai lambda.
 */
class ai final : public component {
    public:
        /*!
         * AI constructor.
         * \param func Function to define AI process.
         * \return void
         */
        inline ai(void func(entity_id, mgr::entity_manager&, mgr::message_manager&, int64_t)) : do_ai(func) {};

        /*!
         * AI destructor.
         * \param void
         * \return void
         */
        inline ~ai() {};

        /*!
         * Run AI function wrapper.
         * \param eid ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param engine_time Current value of the main timer.
         * \return void
         */
        inline void run(entity_id eid, mgr::entity_manager& world, mgr::message_manager& messages, int64_t engine_time) {
            do_ai(eid, world, messages, engine_time);
        };

    private:
        std::function<void(entity_id, mgr::entity_manager&, mgr::message_manager&, int64_t)> do_ai;
};

} //  namespace cmp

} //  namespace wte

#endif
