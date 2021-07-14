/*!
 * WTEngine | File:  spawner.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_SPAWNER_HPP
#define WTE_MGR_SPAWNER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "wtengine/_globals/message.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/manager.hpp"
#include "wtengine/mgr/entities.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class spawner
 * \brief Create or delete entities while the engine is running.
 */
class spawner final : private manager<spawner> {
    friend class interface;

    public:
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
        static const bool add_spawn(const std::string& name, const std::size_t& num_args,
                                    void func(const entity_id&, const msg_arg_list&));

    private:
        /*!
         * \brief Spawn Manager constructor.
         * 
         * Clears the spawner map.
         */
        spawner();

        /*!
         * \brief Spawn Manager destructor.
         * 
         * Clears the spawner map.
         */
        ~spawner();

        /*!
         * \brief Process spawns.
         * 
         * Takes spawner messages and creates or deletes entities.
         * 
         * \param messages Spawner messages from main engine loop.
         */
        static void process_messages(const message_container& messages);

        inline static std::map<std::string, std::pair<std::size_t,
            std::function<void(const entity_id&, const msg_arg_list&)>>> spawns = {};
};

} //  namespace mgr

} //  namespace wte

#endif
