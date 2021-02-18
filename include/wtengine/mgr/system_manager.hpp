/*!
 * WTEngine | File:  system_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_SYSTEM_MANAGER_HPP
#define WTE_MGR_SYSTEM_MANAGER_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/sys/system.hpp"
#include "wtengine/mgr/message_manager.hpp"
#include "wtengine/mgr/entity_manager.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class system_manager
 * \brief Store the configured systems and process their runs and dispatches.
 */
class system_manager final : private manager<system_manager> {
    public:
        /*!
         * \brief System Manager constructor.
         * 
         * Clears the systems vector and sets the finalized flag to false.
         */
        system_manager();

        /*!
         * \brief System Manager destructor.
         * 
         * Makes sure the systems are cleared.
         */
        ~system_manager();

        /*!
         * \brief Clear the system manager and allow systems to be loaded again.
         */
        void clear(void);

        /*!
         * \brief Finalize system manager.
         * 
         * Set finalized flag to prevent additional systems from being loaded.
         */
        void finalize(void);

        /*!
         * \brief Check if systems were loaded into the manager.
         * 
         * \return True if empty, false if not.
         */
        bool empty(void);

        /*!
         * \brief Add a new system to the manager.
         * 
         * Enters system into the vector of systems if not.
         * Systems run in the order they were added.
         * Can fail if the system exists or if the game is running.
         * 
         * \param new_system System to add.
         * \return True if added, false if not.
         */
        bool add(sys::system_uptr new_system);

        /*!
         * \brief Run all systems.
         * 
         * Iterate through the system vector and run each.
         * 
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time);

        /*!
         * \brief Process dispatcher components.
         * 
         * Get messages for the entities and pass to each.
         * Keeps checking for responces and will process as well.
         * 
         * \param world Reference to entity manager.
         * \param messages Reference to message manager.
         * \param current_time Current engine time.
         */
        void dispatch(mgr::entity_manager& world,
                             mgr::message_manager& messages,
                             const int64_t& current_time);

        /*!
         * \brief Reloads temporary bitmaps.
         * 
         * Any background or overlay drawing bitmaps will be recreated.
         * Called when the screen is reloaded.
         * 
         * \param world Reference to the entity manager.
         */
        void reload_temp_bitmaps(mgr::entity_manager& world);

        /*!
         * \brief Enable a system.
         * 
         * Toggle a system to enabled so its run member is processed.
         * 
         * \param sys Name of system to enable.
         * \return True if the system was found, false if it was not.
         */
        const bool enable_system(const std::string& sys);

        /*!
         * \brief Disable a system.
         * 
         * Toggle a system to disabled so its run member is skipped.
         * 
         * \param sys Name of system to disable.
         * \return True if the system was found, false if it was not
         */
        const bool disable_system(const std::string& sys);

    private:
        std::vector<sys::system_uptr> systems;  // Store the vector of systems.
        
        bool finalized; //  Flag to disallow loading of additional systems.
};

template <> bool system_manager::manager<system_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
