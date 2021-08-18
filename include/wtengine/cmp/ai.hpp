/*!
 * WTEngine | File:  ai.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_AI_HPP
#define WTE_CMP_AI_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"

#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"

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
         * \brief Create an AI component with enabled only AI.
         * \param func Function to define AI process.
         */
        ai(
            const std::function<void(const entity_id&)>& func
        );

        /*!
         * \brief Create an AI component with enabled and disabled AI.
         * \param func_a Function to define enabled AI process.
         * \param func_b Function to define disabled AI process.
         */
        ai(
            const std::function<void(const entity_id&)>& func_a,
            const std::function<void(const entity_id&)>& func_b
        );

        ~ai() = default;

        /*!
         * \brief Run enabled AI function wrapper.
         * \param eid ID of the entity calling the wrapper.
         */
        void run_enabled(
            const entity_id& e_id
        );

        /*!
         * \brief Run disabled AI function wrapper.
         * \param eid ID of the entity calling the wrapper.
         */
        void run_disabled(
            const entity_id& e_id
        );

        /*!
         * \brief Check if the entity is enabled.
         * \return True if enabled, false if not.
         */
        const bool status(void) const;

        /*!
         * \brief Set to enabled.
         */
        void enable(void);

        /*!
         * \brief Set to disabled.
         */
        void disable(void);

    private:
        bool enabled;  //  Enabled flag.
        std::function<void(const entity_id&)> enabled_ai;   //  AI to run when enabled.
        std::function<void(const entity_id&)> disabled_ai;  //  AI to run when disabled.
};

} //  namespace cmp

} //  namespace wte

#endif
