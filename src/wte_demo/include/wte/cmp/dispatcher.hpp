/*!
 * \brief WTEngine | File:  dispatcher.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Dispatcher component - tag to be processed by logic system.
 */

#ifndef WTE_CMP_DISPATCHER_HPP
#define WTE_CMP_DISPATCHER_HPP

#include <functional>

#include "component.hpp"
#include "../mgr/entity_manager.hpp"
#include "../message.hpp"

namespace wte
{

namespace cmp
{

//! Dispatcher component
/*!
 * Tag components to be processed by the Logic system.
 * Define message processing in handle_msg lambda.
 */
class dispatcher : public component {
    public:
        std::function<void(entity, mgr::entity_manager&, message_container, int64_t)> handle_msg;
};

} //  namespace cmp

} //  namespace wte

#endif
