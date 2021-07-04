/*!
 * WTEngine | File:  input_button.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_INPUT_BUTTON_HPP
#define WTE_CMP_INPUT_BUTTON_HPP

#include <functional>
#include <cassert>

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/entities.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/_globals/input_flags.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class input_button
 * \brief Binds a button to the entity.
 * 
 * Tag an entity to be processed by the Input system.
 */
class input_button final : public component {
    public:
        /*!
         * \brief Input button constructor.
         * 
         * \param button Button flag enum to bind to.
         * \param func_a Button on down function.
         * \param func_b Button on up function.
         */
        input_button(
            const std::size_t button,
            std::function<void(const entity_id&)> func_a,
            std::function<void(const entity_id&)> func_b
        );

        /*!
         * \brief Input button destructor.
         */
        inline ~input_button() {};

        /*!
         * \brief Get the button flag bounded to this component.
         * 
         * \return The button flag enum value.
         */
        const std::size_t get_flag(void) const;

        /*!
         * \brief Run button on down event.
         * 
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void on_down(const entity_id& e_id);

        /*!
         * \brief Run button on up event.
         * 
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void on_up(const entity_id& e_id);

    private:
        std::size_t button_flag;

        std::function<void(const entity_id&)> button_down;

        std::function<void(const entity_id&)> button_up;
};

} //  namespace cmp

} //  namespace wte

#endif
