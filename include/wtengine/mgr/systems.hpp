/*!
 * WTEngine | File:  systems.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_SYSTEMS_HPP
#define WTE_MGR_SYSTEMS_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/sys/system.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/entities.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class systems
 * \brief Store the configured systems and process their runs and dispatches.
 */
class systems final : private manager<systems> {
    friend class wte_main;

    public:
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
        static const bool add(sys::system_uptr new_system);

        /*!
         * \brief Enable a system.
         * 
         * Toggle a system to enabled so its run member is processed.
         * 
         * \param sys Name of system to enable.
         * \return True if the system was found, false if it was not.
         */
        static const bool enable_system(const std::string& sys);

        /*!
         * \brief Disable a system.
         * 
         * Toggle a system to disabled so its run member is skipped.
         * 
         * \param sys Name of system to disable.
         * \return True if the system was found, false if it was not
         */
        static const bool disable_system(const std::string& sys);

        /*!
         * \brief Clear the system manager and allow systems to be loaded again.
         */
        static void clear(void);

        /*!
         * \brief Finalize system manager.
         * 
         * Set finalized flag to prevent additional systems from being loaded.
         */
        static void finalize(void);

        /*!
         * \brief Check if systems were loaded into the manager.
         * 
         * \return True if empty, false if not.
         */
        static const bool empty(void);

        /*!
         * \brief Run all systems.
         * 
         * Iterate through the system vector and run each.
         */
        static void run(void);

        /*!
         * \brief Process dispatcher components.
         * 
         * Get messages for the entities and pass to each.
         * Keeps checking for responces and will process as well.
         */
        static void dispatch(void);

        /*!
         * \brief Reloads temporary bitmaps.
         * 
         * Any background or overlay drawing bitmaps will be recreated.
         * Called when the screen is reloaded.
         */
        static void reload_temp_bitmaps(void);

    private:
        /*!
         * \brief System Manager constructor.
         * 
         * Clears the systems vector and sets the finalized flag to false.
         */
        systems();

        /*!
         * \brief System Manager destructor.
         * 
         * Makes sure the systems are cleared.
         */
        ~systems();

        inline static std::vector<sys::system_uptr> _systems = {};  // Store the vector of systems.

        inline static bool finalized = false; //  Flag to disallow loading of additional systems.
};

} //  namespace mgr

} //  namespace wte

#endif
