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

#include "manager.hpp"
#include "../sys/system.hpp"
#include "message_manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace mgr
{

//!  Iterator for addressing systems.
typedef std::vector<sys::system_uptr>::iterator system_iterator;
//!  Constant iterator for addressing systems.
typedef std::vector<sys::system_uptr>::const_iterator system_citerator;

//! System Manager class
/*!
 * Store the configured systems and process their runs and dispatches.
 */
class system_manager final : public manager<system_manager> {
    public:
        /*!
         * System Manager constructor.
         * Clears the systems vector and sets the finalized flag to false.
         * \param void
         * \return void
         */
        inline system_manager() : finalized(false) { systems.clear(); };

        /*!
         * System Manager destructor.
         * Makes sure the systems are cleared.
         * \param void
         * \return void
         */
        inline ~system_manager() { systems.clear(); };

        /*!
         * Clear the system manager and allow systems to be loaded again.
         * \param void
         * \return void
         */
        inline void clear(void) {
            systems.clear();
            finalized = false;
        };

        /*!
         * \brief Finalize system manager.
         * Set finalized flag to prevent additional systems from being loaded.
         * \param void
         * \return void
         */
        inline void finalize(void) { finalized = true; };

        /*!
         * Check if systems were loaded into the manager.
         * \param void
         * \return True if empty, false if not.
         */
        inline bool empty(void) { return systems.empty(); };

        /*!
         * \brief Add a new system to the manager.
         * Enters system into the vector of systems if not.
         * Systems run in the order they were added.
         * Can fail if the system exists or if the game is running.
         * \param new_system System to add.
         * \return True if added, false if not.
         */
        inline bool add(sys::system_uptr new_system) {
            if(finalized == true) return false;

            for(auto & it : systems)
                if((it)->get_name() == new_system->get_name()) return false;

            systems.push_back(std::move(new_system));
            return true;
        };

        /*!
         * \brief Run all systems.
         * Iterate through the system vector and run each.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        int64_t current_time) {
            for(auto & it : systems)
                if((it)->is_enabled()) (it)->run(world, messages, current_time);
        };

        /*!
         * \brief Process dispatcher components.
         * Get messages for the entities and pass to each.
         * Keeps checking for responces and will process as well.
         * \param world Reference to entity manager.
         * \param messages Reference to message manager.
         * \param current_time Current engine time.
         * \return void
         */
        inline void dispatch(mgr::entity_manager& world,
                             mgr::message_manager& messages,
                             int64_t current_time) {
            component_container<cmp::dispatcher> dispatch_components =
                world.set_components<cmp::dispatcher>();

            while(true) {
                message_container temp_msgs = messages.get_messages("entities");
                if(temp_msgs.empty()) break;  //  No messages, end while(true) loop.

                for(auto & c_it : dispatch_components) {
                    for(auto m_it = temp_msgs.begin(); m_it != temp_msgs.end();) {
                        if(m_it->get_to() == world.get_component<cmp::name>(c_it.first)->name_str) {
                            c_it.second->proc_msg(c_it.first, world, messages, current_time, *m_it);
                            m_it = temp_msgs.erase(m_it);
                        } else m_it++;
                    }
                    if(temp_msgs.empty()) break;  //  No messages left, end comp loop.
                }
            }
        };

        /*!
         * \brief Enable a system.
         * Toggle a system to enabled so its run member is processed.
         * \param sys Name of system to enable.
         * \return True if the system was found, false if it was not.
         */
        inline const bool enable_system(std:: string sys) {
            for(auto & it : systems) {
                if((it)->get_name() == sys) (it)->enable();
                return true;
            }
            return false;
        };

        /*!
         * \brief Disable a system.
         * Toggle a system to disabled so its run member is skipped.
         * \param sys Name of system to disable.
         * \return True if the system was found, false if it was not
         */
        inline const bool disable_system(std:: string sys) {
            for(auto & it : systems) {
                if((it)->get_name() == sys) (it)->disable();
                return true;
            }
            return false;
        };

    private:
        std::vector<sys::system_uptr> systems;  // Store the vector of systems.
        
        bool finalized; //  Flag to disallow loading of additional systems.
};

template <> inline bool system_manager::manager<system_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
