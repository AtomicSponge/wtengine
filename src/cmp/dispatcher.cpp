/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/cmp/dispatcher.hpp"

namespace wte::cmp {

/*
 *
 */
dispatcher::dispatcher(
    const std::function<void(const entity_id&, const message&)>& func
) : handle_msg(func) {}

}  //  end namespace wte::cmp
