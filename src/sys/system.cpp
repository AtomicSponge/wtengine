/*!
 * WTEngine | File:  system.cpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <memory>

#include "wtengine/system.cpp"

namespace wte
{

namespace sys
{

//!  Enable the system
void system::enable(void) { enabled = true; };
//!  Check if the system is enabled
const bool system::is_enabled(void) const { return enabled; };

//!  Get the system name
const std::string system::get_name(void) const { return name; };

//!  Override this to create custom run method
virtual void run(mgr::entity_manager&,
                    mgr::message_manager&,
                    const int64_t&) = 0;

} //  namespace sys

} //  namespace wte
