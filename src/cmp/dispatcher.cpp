/*!
 * wtengine | File:  dispatcher.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
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
