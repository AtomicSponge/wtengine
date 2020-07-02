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

/*!
 * \class AI component
 * Tag components to be processed by the Logic system.
 * Define logic in do_ai lambda.
 */
class ai final : public component {
    public:
        /*!
         * \brief AI constructor.
         * Create an AI component with enabled only AI.
         * \param func Function to define AI process.
         * \return void
         */
        inline ai(void func(const entity_id&,
                            mgr::entity_manager&,
                            mgr::message_manager&,
                            const int64_t&)) :
        enabled_ai(func), disabled_ai([](const entity_id& e_id,
                                         mgr::entity_manager& world,
                                         mgr::message_manager& messages,
                                         const int64_t& engine_time){}) {};

        /*!
         * \brief AI constructor.
         * Create an AI component with enabled and disabled AI.
         * \param func_a Function to define enabled AI process.
         * \param func_b Function to define disabled AI process.
         * \return void
         */
        inline ai(void func_a(const entity_id&,
                              mgr::entity_manager&,
                              mgr::message_manager&,
                              const int64_t&),
                  void func_b(const entity_id&,
                              mgr::entity_manager&,
                              mgr::message_manager&,
                              const int64_t&)) :
        enabled_ai(func_a), disabled_ai(func_b) {};

        /*!
         * AI destructor.
         * \param void
         * \return void
         */
        inline ~ai() {};

        /*!
         * Run enabled AI function wrapper.
         * \param eid ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param engine_time Current value of the main timer.
         * \return void
         */
        inline void run_enabled(const entity_id& eid,
                                mgr::entity_manager& world,
                                mgr::message_manager& messages,
                                const int64_t& engine_time) {
            enabled_ai(eid, world, messages, engine_time);
        };

        /*!
         * Run disabled AI function wrapper.
         * \param eid ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param engine_time Current value of the main timer.
         * \return void
         */
        inline void run_disabled(const entity_id& eid,
                                 mgr::entity_manager& world,
                                 mgr::message_manager& messages,
                                 const int64_t& engine_time) {
            disabled_ai(eid, world, messages, engine_time);
        };

    private:
        std::function<void(const entity_id&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> enabled_ai;

        std::function<void(const entity_id&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> disabled_ai;
};

} //  namespace cmp

} //  namespace wte

#endif
