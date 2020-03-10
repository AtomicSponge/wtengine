/*!
 * \brief WTEngine | File:  system_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details System manager.
 */

#ifndef WTE_MGR_SYSTEM_MANAGER_HPP
#define WTE_MGR_SYSTEM_MANAGER_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>
#include <stdexcept>

#include "manager.hpp"
#include "..\systems\system.hpp"
#include "message_manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace mgr
{

typedef std::vector<sys::system_uptr>::iterator system_iterator;
typedef std::vector<sys::system_uptr>::const_iterator system_citerator;

//! system_manager class
/*!
  Store the configured systems and process their runs and dispatches
*/
class system_manager final : public manager<system_manager> {
    public:
        //!  System Manager constructor
        //!  Clears the systems vector and sets the finalized flag to false
        inline system_manager() : finalized(false) { systems.clear(); };
        //!  System Manager destructor
        //!  Makes sure the systems are cleared
        inline ~system_manager() { systems.clear(); };

        //!  Clear the system manager and allow systems to be loaded again
        inline void clear(void) {
            systems.clear();
            finalized = false;
        };

        //!  Finalize system manager
        //!  Set finalized flag to prevent additional systems from being loaded
        inline void finalize(void) { finalized = true; };

        //!  Add a new system to the manager
        void add(sys::system_uptr);
        //!  Run all systems
        void run(entity_manager&, mgr::message_manager&, int64_t);
        //!  Process dispatch for all systems
        void dispatch(entity_manager&, mgr::message_manager&);

        //!  Enable a system
        const bool enable_system(std::string);
        //!  Disable a system
        const bool disable_system(std::string);

    private:
        std::vector<sys::system_uptr> systems;  // Store the vector of systems
        
        bool finalized; //  Flag to disallow loading of additional systems
};

template <> inline bool system_manager::manager<system_manager>::initialized = false;

/*!
  Checks to see if a similar named system is already loaded
  Enters system into the vector of systems if not
  Systems run in the order they were added
*/
inline void system_manager::add(sys::system_uptr new_system) {
    if(finalized == true) throw std::runtime_error("System manager already configured - Can't add additional system!");

    for(system_citerator it = systems.begin(); it != systems.end(); it++) {
        if((*it)->get_name() == new_system->get_name()) throw std::runtime_error("System already loaded!");
    }

    systems.push_back(std::move(new_system));
}

/*!
  Iterate through the system vector and run each
  Throw error if no systems have been loaded
*/
inline void system_manager::run(entity_manager& entities, mgr::message_manager& messages, int64_t current_time) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_citerator it = systems.begin(); it != systems.end(); it++) {
        if((*it)->is_enabled()) (*it)->run(entities, messages, current_time);
    }
}

/*!
  Checks each system for its name and sends corresponding messages
  Throw error if no systems have been loaded
*/
inline void system_manager::dispatch(entity_manager& entities, mgr::message_manager& messages) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_citerator it = systems.begin(); it != systems.end(); it++) {
        (*it)->dispatch(entities, messages.get_messages((*it)->get_name()));
    }
}

/*!
  Toggle a system to enabled so it's run member is processed
  Returns true if the system was found, false if it was not
*/
inline const bool system_manager::enable_system(std:: string sys) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_iterator it = systems.begin(); it != systems.end(); it++) {
        if((*it)->get_name() == sys) (*it)->enable();
        return true;
    }
    return false;
}

/*!
  Toggle a system to disabled so it's run member is skipped
  Dispatching will still be processed
  Returns true if the system was found, false if it was not
*/
inline const bool system_manager::disable_system(std:: string sys) {
    if(systems.empty()) throw std::runtime_error("No systems have been loaded!");

    for(system_iterator it = systems.begin(); it != systems.end(); it++) {
        if((*it)->get_name() == sys) (*it)->disable();
        return true;
    }
    return false;
}

} //  namespace mgr

} //  namespace wte

#endif
