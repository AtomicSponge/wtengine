/*!
 * WTEngine | File:  systems.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
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

#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"
#include "wtengine/sys/system.hpp"

namespace wte {

class engine;

namespace mgr {

/*!
 * \class systems
 * \brief Store the configured systems and process their runs and dispatches.
 */
class systems final : private manager<systems> {
    friend class wte::engine;

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
        static const bool add(
            sys::system_uptr new_system
        );

    private:
        systems() = default;
        ~systems() = default;

        /*
         * Clear the system manager and allow systems to be loaded again.
         */
        static void clear(void);

        /*
         * Set finalized flag to prevent additional systems from being loaded.
         */
        static void finalize(void);

        /*
         * Check if systems were loaded into the manager.
         */
        static const bool empty(void);

        /*
         * \brief Run all systems.
         */
        static void run(void);

        /*
         * Process dispatcher components. 
         * Get messages for the entities and pass to each.
         * Keeps checking for responces and will process as well.
         */
        static void dispatch(void);

        // Store the vector of systems.
        static std::vector<sys::system_uptr> _systems;
        //  Flag to disallow loading of additional systems.
        static bool finalized;
};

} //  namespace mgr

} //  namespace wte

#endif
