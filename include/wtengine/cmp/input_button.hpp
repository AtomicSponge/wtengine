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
#include "wtengine/mgr/entity_manager.hpp"
#include "wtengine/mgr/message_manager.hpp"
#include "wtengine/_globals/input_flags.hpp"

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
            void func_a(const entity_id&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&),
            void func_b(const entity_id&,
                        mgr::entity_manager&,
                        mgr::message_manager&,
                        const int64_t&)
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
        void on_down(const entity_id& e_id,
                     mgr::entity_manager& world,
                     mgr::message_manager& messages,
                     const int64_t& current_time);

        /*!
         * \brief Run button on up event.
         * 
         * \param e_id Entity ID
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void on_up(const entity_id& e_id,
                   mgr::entity_manager& world,
                   mgr::message_manager& messages,
                   const int64_t& current_time);

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
