/*!
 * WTEngine | File:  dispatcher.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
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

/*!
 * \class dispatcher
 * \brief Tag components to be processed by the Logic system.
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    public:
        /*!
         * Dispatcher constructor.
         * \param func Function to define message processing.
         */
        inline dispatcher(void func(const entity_id&,
                                    const message&,
                                    mgr::entity_manager&,
                                    mgr::message_manager&,
                                    const int64_t&)) : handle_msg(func) {};

        /*!
         * Dispatched destructor.
         */
        inline ~dispatcher() {};

        /*!
         * Run dispatcher function wrapper.
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \param msg Message to process.
         */
        inline void proc_msg(const entity_id& e_id,
                             const message& msg,
                             mgr::entity_manager& world,
                             mgr::message_manager& messages,
                             const int64_t& current_time) {
            handle_msg(e_id, msg, world, messages, current_time);
        };

    private:
        std::function<void(const entity_id&,
                           const message&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           const int64_t&)> handle_msg;
};

} //  namespace cmp

} //  namespace wte

#endif
