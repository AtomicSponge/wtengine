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

#include "wtengine/cmp/component.hpp"
#include "wtengine/mgr/message_manager.hpp"
#include "wtengine/mgr/entity_manager.hpp"
#include "wtengine/message.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class dispatcher
 * \brief Tag components to be processed by the Logic system.
 * 
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    public:
        /*!
         * \brief Dispatcher constructor.
         * 
         * \param func Function to define message processing.
         */
        dispatcher(void func(const entity_id&,
                             const message&,
                             mgr::entity_manager&,
                             mgr::message_manager&,
                             const int64_t&));

        /*!
         * \brief Dispatched destructor.
         */
        inline ~dispatcher() {};

        /*!
         * \brief Run dispatcher function wrapper.
         * 
         * \param e_id ID of the entity calling the wrapper.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \param msg Message to process.
         */
        void proc_msg(const entity_id& e_id,
                      const message& msg,
                      mgr::entity_manager& world,
                      mgr::message_manager& messages,
                      const int64_t& current_time);

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
