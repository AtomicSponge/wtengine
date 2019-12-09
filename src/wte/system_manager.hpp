/*
  WTEngine
  By:  Matthew Evans
  File:  system_manager.hpp

  See LICENSE.txt for copyright information

  System Manager
*/

#ifndef WTE_ECS_SYSTEM_MANAGER_HPP
#define WTE_ECS_SYSTEM_MANAGER_HPP

#include <vector>
#include <iterator>
#include <memory>
#include <stdexcept>

#include "message_queue.hpp"
#include "systems/system.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace ecs
{

typedef std::vector<sys::system_uptr>::iterator system_iterator;

//! system_manager class
/*!
  Store the configured systems and process their runs and dispatches
*/
class system_manager {
    public:
        void add(sys::system_uptr);                                 /*!< Add a new system */
        void run(entity_manager&, msg::message_queue&, int64_t);    /*!< Run all systems */
        void dispatch(entity_manager&, msg::message_queue&);        /*!< Dispatch to all systems */

    private:
        std::vector<sys::system_uptr> systems;                      /*!< Store the vector of systems */
};

//! Add a new system to the manager
/*!
  Systems run in the order they were added
*/
inline void system_manager::add(sys::system_uptr new_system) {
    systems.push_back(std::move(new_system));
}

//! Run all systems
/*!
  Throw error if no systems have been loaded
*/
inline void system_manager::run(entity_manager& entities, msg::message_queue& messages, int64_t current_time) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_iterator it = systems.begin(); it != systems.end(); it++) {
        (*it)->run(entities, messages, current_time);
    }
}

//! Process dispatch for all systems
/*!
  Throw error if no systems have been loaded
*/
inline void system_manager::dispatch(entity_manager& entities, msg::message_queue& messages) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_iterator it = systems.begin(); it != systems.end(); it++) {
        (*it)->dispatch(entities, messages);
    }
}

} //  namespace ecs

} //  namespace wte

#endif
