/*!
 * WTEngine | File:  dispatcher.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_DISPATCHER_HPP
#define WTE_CMP_DISPATCHER_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"

#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/alert.hpp"

namespace wte::mgr {
    class systems;
}

namespace wte::cmp {

/*!
 * \class dispatcher
 * \brief Tag components to be processed by the Logic system.
 * 
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    friend class mgr::systems;

    public:
        /*!
         * \brief Create a new Dispatcher component.
         * \param func Function to define message processing.
         */
        dispatcher(
            const std::function<void(
                const entity_id&,
                const message&)>& func
        );

        /*!
         * \brief Dispatched destructor.
         */
        ~dispatcher() = default;

    private:
        /*
         * Run dispatcher function wrapper.
         */
        void proc_msg(
            const entity_id& e_id,
            const message& msg
        );

        //  Message handler.
        std::function<void(const entity_id&, const message&)> handle_msg;
};

}  //  end namespace wte::cmp

#endif
