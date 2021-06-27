/*!
 * WTEngine | File:  ai.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_AI_HPP
#define WTE_CMP_AI_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/entities.hpp"
#include "wtengine/mgr/messages.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class ai
 * \brief Tag components to be processed by the Logic system.
 * 
 * Allows functions to be created to define the enabled or disabled logic.
 */
class ai final : public component {
    public:
        /*!
         * \brief AI constructor.
         * 
         * Create an AI component with enabled only AI.
         * 
         * \param func Function to define AI process.
         */
        ai(void func(const entity_id&));

        /*!
         * \brief AI constructor.
         * 
         * Create an AI component with enabled and disabled AI.
         * 
         * \param func_a Function to define enabled AI process.
         * \param func_b Function to define disabled AI process.
         */
        ai(void func_a(const entity_id&),
           void func_b(const entity_id&));

        /*!
         * \brief AI destructor.
         */
        inline ~ai() {};

        /*!
         * \brief Run enabled AI function wrapper.
         * 
         * \param eid ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param engine_time Current value of the main timer.
         */
        void run_enabled(const entity_id& e_id);

        /*!
         * \brief Run disabled AI function wrapper.
         * 
         * \param eid ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param engine_time Current value of the main timer.
         */
        void run_disabled(const entity_id& e_id);

    private:
        std::function<void(const entity_id&)> enabled_ai;

        std::function<void(const entity_id&)> disabled_ai;
};

} //  namespace cmp

} //  namespace wte

#endif
