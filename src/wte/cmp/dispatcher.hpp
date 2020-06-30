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

/*!
 * \class Dispatcher component
 * Tag components to be processed by the Logic system.
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    public:
        /*!
         * Dispatcher constructor.
         * \param func Function to define message processing.
         * \return void
         */
        inline dispatcher(void func(entity_id,
                                    message&,
                                    mgr::entity_manager&,
                                    mgr::message_manager&,
                                    int64_t)) : handle_msg(func) {};

        /*!
         * Dispatched destructor.
         * \param void
         * \return void
         */
        inline ~dispatcher() {};

        /*!
         * Run dispatcher function wrapper.
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \param msg Message to process.
         * \return void
         */
        inline void proc_msg(entity_id eid,
                             message& msg,
                             mgr::entity_manager& world,
                             mgr::message_manager& messages,
                             int64_t current_time) {
            handle_msg(eid, msg, world, messages, current_time);
        };

    private:
        std::function<void(entity_id,
                           message&,
                           mgr::entity_manager&,
                           mgr::message_manager&,
                           int64_t)> handle_msg;
};

} //  namespace cmp

} //  namespace wte

#endif
