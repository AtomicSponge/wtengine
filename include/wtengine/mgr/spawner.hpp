/*!
 * WTEngine | File:  spawner.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_SPAWNER_HPP
#define WTE_MGR_SPAWNER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/message.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte {
    class engine;
}

namespace wte::mgr {

/*!
 * \class spawner
 * \brief Create or delete entities while the engine is running.
 */
class spawner final : private manager<spawner> {
    friend class wte::engine;

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
        static const bool add(
            const std::string& name,
            const std::size_t& num_args,
            const std::function<void(const entity_id&, const msg_args&)>& func
        );

        /*!
         * \brief Delete a spawn.
         * \param name Name of spawn to delete.
         */
        static const bool remove(const std::string& name);

    private:
        spawner() = default;
        ~spawner() = default;

        /*
         * Takes spawner messages and processes.
         */
        static void process_messages(const message_container& messages);

        static std::map<
            const std::string,
            std::pair<
                const std::size_t,
                const std::function<void(const entity_id&, const msg_args&)>
        >> spawns;
};

}  //  end namespace wte::mgr

#endif
