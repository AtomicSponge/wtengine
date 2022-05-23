/*!
 * WTEngine | File:  dispatcher.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_CMP_DISPATCHER_HPP
#define WTE_CMP_DISPATCHER_HPP

#include <functional>

#include "wtengine/cmp/component.hpp"

#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte::mgr {
    class messages;
}

namespace wte::cmp {

/*!
 * \class dispatcher
 * \brief Tag components to be processed by the Logic system.
 * 
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
    friend class mgr::messages;

    public:
        /*!
         * \brief Create a new Dispatcher component.
         * \param func Function to define message processing.
         */
        dispatcher(const std::function<void(const entity_id&, const message&)>& func);

        dispatcher() = delete;    //!<  Delete default constructor.
        ~dispatcher() = default;  //!<  Default destructor.

    private:
        //  Message handler.
        const std::function<void(const entity_id&, const message&)> handle_msg;
};

}  //  end namespace wte::cmp

#endif
