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

//! AI component
/*!
 * Tag components to be processed by the Logic system.
 * Define logic in do_ai lambda.
 */
class ai final : public component {
    public:
        inline ai(void func(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)) : do_ai(func) {};
        inline ~ai() {};

    private:
        std::function<void(entity, mgr::entity_manager&, mgr::message_manager&, int64_t)> do_ai;
};

} //  namespace cmp

} //  namespace wte

#endif
