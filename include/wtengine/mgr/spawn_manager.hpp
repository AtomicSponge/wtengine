/*!
 * WTEngine | File:  spawn_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MGR_SPAWN_MANAGER_HPP
#define WTE_MGR_SPAWN_MANAGER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "wtengine/manager.hpp"
#include "wtengine/entity_manager.hpp"
#include "wtengine/../message.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class spawn_manager
 * \brief Create or delete entities while the engine is running.
 */
class spawn_manager final : private manager<spawn_manager> {
    public:
        /*!
         * \brief Spawn Manager constructor.
         * 
         * Clears the spawner map.
         */
        spawn_manager();

        /*!
         * \brief Spawn Manager destructor.
         * 
         * Clears the spawner map.
         */
        ~spawn_manager();

        /*!
         * \brief Process spawns.
         * 
         * Takes spawner messages and creates or deletes entities.
         * 
         * \param messages Spawner messages from main engine loop.
         * \param world Reference to the entity manager.
         */
        void process(const message_container& messages, entity_manager& world);

        /*!
         * \brief Add a spawn to the spawner map.
         * 
         * Stores a function in the map for entity creation on demand.
         * 
         * \param name Reference name for the spwaner item.
         * \param num_args Number of arguments the spawn accepts.
         * \param func Function for creating the entity.
         * \return True if inserted into the spawn map, false if not.
         */
        const bool add_spawn(const std::string& name, const std::size_t& num_args,
                                    void func(const entity_id&, entity_manager&, const msg_arg_list&));

    private:
        std::map<std::string, std::pair<std::size_t,
            std::function<void(const entity_id&, entity_manager&, const msg_arg_list&)>>> spawner;
};

template <> bool spawn_manager::manager<spawn_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
