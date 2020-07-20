/*!
 * \brief WTEngine | File:  input_button.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input button component.  Tag to be processed by input system.
 */

#ifndef WTE_CMP_INPUT_BUTTON_HPP
#define WTE_CMP_INPUT_BUTTON_HPP

#include <functional>
#include <cassert>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class input_button
 * \brief Tag an entity to be processed by the Input system.
 * Binds a button to the entity.
 */
class input_button final : public component {
    public:
        /*!
         * Input button constructor.
         * \param button Button flag enum to bind to.
         * \param func_a Button on down function.
         * \param func_b Button on up function.
         * \return void
         */
        inline input_button(
            const std::size_t button,
            void func_a(const entity_id&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&),
            void func_b(const entity_id&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&)
        ) : button_flag(button), button_down(func_a), button_up(func_b) {
            assert(button_flag < WTE_MAX_INPUT_BUTTON_FLAGS);
        };

        /*!
         * Input button destructor.
         * \param void
         * \return void
         */
        inline ~input_button() {};

        /*!
         * Get the button flag bounded to this component.
         * \param void
         * \return The button flag enum value.
         */
        inline const std::size_t get_flag(void) const {
            return button_flag;
        };

        /*!
         * Run button on down event.
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        inline void on_down(const entity_id& e_id,
                            mgr::entity_manager& world,
                            mgr::message_manager& messages,
                            const int64_t& current_time) {
            button_down(e_id, world, messages, current_time);
        };

        /*!
         * Run button on up event.
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        inline void on_up(const entity_id& e_id,
                          mgr::entity_manager& world,
                          mgr::message_manager& messages,
                          const int64_t& current_time) {
            button_up(e_id, world, messages, current_time);
        };

    private:
        std::size_t button_flag;

        std::function<void(const entity_id&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> button_down;

        std::function<void(const entity_id&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> button_up;
};

} //  namespace cmp

} //  namespace wte

#endif
