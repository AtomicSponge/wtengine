/*!
 * \brief WTEngine | File:  input_handler.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input handler component.  Tag to be processed by input system.
 */

#ifndef WTE_CMP_INPUT_HANDLER_HPP
#define WTE_CMP_INPUT_HANDLER_HPP

#include <map>
#include <utility>
#include <functional>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../mgr/message_manager.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Input Handler component
 * Tag an entity to be processed by the Input system
 */
class input_handler final : public component {
    public:
        /*!
         * Input handler constructor.
         * \param void
         * \return void
         */
        inline input_handler() {
            input_map.clear();
        };

        /*!
         * Input handler destructor.
         * \param void
         * \return void
         */
        inline ~input_handler() {
            input_map.clear();
        };

        /*!
         * \brief Add input handler.
         * Adds a handler with input down only handling.
         * \param button WTE_INPUT enum item.
         * \param func_on Function to run when the button is pressed.
         * \return True if added, false if not.
         */
        inline const bool add_handle_on(
            const std::size_t button,
            void func_on(const entity,
                         mgr::entity_manager&,
                         mgr::message_manager&,
                         const int64_t)
        ) {
            auto func_off =
                [](const entity plr_id,
                   mgr::entity_manager& world,
                   mgr::message_manager& messages,
                   const int64_t engine_time) {};
            auto ret = input_map.insert(
                std::make_pair(button, std::make_pair(func_on, func_off))
            );
            return ret.second;
        };

        /*!
         * \brief Add input handler.
         * Adds a handler with input up only handling.
         * \param button WTE_INPUT enum item.
         * \param func_off Function to run when the button is not pressed.
         * \return True if added, false if not.
         */
        inline const bool add_handle_off(
            const std::size_t button,
            void func_off(const entity,
                          mgr::entity_manager&,
                          mgr::message_manager&,
                          const int64_t)
        ) {
            auto func_on =
                [](const entity plr_id,
                   mgr::entity_manager& world,
                   mgr::message_manager& messages,
                   const int64_t engine_time) {};
            auto ret = input_map.insert(
                std::make_pair(button, std::make_pair(func_on, func_off))
            );
            return ret.second;
        };

        /*!
         * \brief Add input handler.
         * Adds a handler with input up and down handling.
         * \param button WTE_INPUT enum item.
         * \param func_on Function to run when the button is pressed.
         * \param func_off Function to run when the button is not pressed.
         * \return True if added, false if not.
         */
        inline const bool add_handle_on_off(
            const std::size_t button,
            void func_on(const entity,
                         mgr::entity_manager&,
                         mgr::message_manager&,
                         const int64_t),
            void func_off(const entity,
                          mgr::entity_manager&,
                          mgr::message_manager&,
                          const int64_t)
        ) {
            auto ret = input_map.insert(
                std::make_pair(button, std::make_pair(func_on, func_off))
            );
            return ret.second;
        };

        //! Store the input map.
        std::map<const std::size_t, std::pair<
            std::function<void(const entity,
                               mgr::entity_manager&,
                               mgr::message_manager&,
                               const int64_t)>,
            std::function<void(const entity,
                               mgr::entity_manager&,
                               mgr::message_manager&,
                               const int64_t)>
        >> input_map;
};

} //  namespace cmp

} //  namespace wte

#endif
