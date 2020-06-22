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
         *
         */
        inline input_handler() {};

        /*!
         *
         */
        inline ~input_handler() {};

        /*!
         *
         */
        inline const bool add_handle(
            std::size_t b,
            void func_on(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)
        ) {
            auto func_off =
                [](entity plr_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t engine_time) {};
            auto ret = input_map.insert(std::make_pair(b,std::make_pair(func_on, func_off)));
            return ret.second;
        };

        /*!
         *
         */
        inline const bool add_handle(
            std::size_t b,
            void func_on(entity, mgr::entity_manager&, mgr::message_manager&, int64_t),
            void func_off(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)
        ) {
            auto ret = input_map.insert(std::make_pair(b,std::make_pair(func_on, func_off)));
            return ret.second;
        };

        std::map<std::size_t, std::pair<
            std::function<void(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)>,
            std::function<void(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)>
        >> input_map;
};

} //  namespace cmp

} //  namespace wte

#endif
