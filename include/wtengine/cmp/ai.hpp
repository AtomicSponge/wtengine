/*!
 * wtengine | File:  ai.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_CMP_AI_HPP
#define WTE_CMP_AI_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"

#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte::sys {
    class logic;
}

namespace wte::cmp {

/*!
 * \class ai
 * \brief Tag components to be processed by the Logic system.
 * 
 * Allows functions to be created to define the enabled or disabled logic.
 */
class ai final : public component {
    friend class sys::logic;

    public:
        /*!
         * \brief Create an AI component with enabled only AI.
         * \param func Function to define AI process.
         */
        ai(const std::function<void(const entity_id&)>& func);

        /*!
         * \brief Create an AI component with enabled and disabled AI.
         * \param func_a Function to define enabled AI process.
         * \param func_b Function to define disabled AI process.
         */
        ai(
            const std::function<void(const entity_id&)>& func_a,
            const std::function<void(const entity_id&)>& func_b
        );

        ai() = delete;    //!<  Delete default constructor.
        ~ai() = default;  //!<  Default destructor.

        bool enabled;     //!<  Flag to enable or disable the entity.

    private:
        const std::function<void(const entity_id&)> enabled_ai;   //  AI to run when enabled.
        const std::function<void(const entity_id&)> disabled_ai;  //  AI to run when disabled.
};

}  //  end namespace wte::cmp

#endif
