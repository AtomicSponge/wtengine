/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_DISPATCHER_HPP)
#define WTE_CMP_DISPATCHER_HPP

#include <functional>

#include "silvergun/cmp/component.hpp"

#include "silvergun/_globals/message.hpp"
#include "silvergun/mgr/messages.hpp"
#include "silvergun/mgr/world.hpp"

namespace wte::mgr {
  class messages;
}

namespace wte::cmp {

/*!
 * \class dispatcher
 * \brief Tag components to receive messages.
 * 
 * Define message processing in handle_msg lambda.
 */
class dispatcher final : public component {
  friend class mgr::messages;

  private:
    //  Message handler.
    const std::function<void(const entity_id&, const message&)> handle_msg;

  public:
    /*!
     * \brief Create a new Dispatcher component.
     * \param func Function to define message processing.
     */
    dispatcher(const std::function<void(const entity_id&, const message&)>& func) : handle_msg(func) {};

    dispatcher() = delete;    //  Delete default constructor.
    ~dispatcher() = default;  //  Default destructor.
};

}  //  end namespace wte::cmp

#endif
