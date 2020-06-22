/*!
 * \brief WTEngine | File:  spawn_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Spawn manager.
 */

#ifndef WTE_MGR_SPAWN_MANAGER_HPP
#define WTE_MGR_SPAWN_MANAGER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "manager.hpp"
#include "entity_manager.hpp"
#include "../message.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class spawn_manager class
 * Spawn stuff
 */
class spawn_manager final : public manager<spawn_manager> {
    public:
        /*!
         * \brief Spawn Manager constructor.
         * Clears the spawner map.
         * \param void
         * \return void
         */
        inline spawn_manager() {
            spawner.clear();
        };

        /*!
         * \brief Spawn Manager destructor.
         * Clears the spawner map.
         * \param void
         * \return void
         */
        inline ~spawn_manager() {
            spawner.clear();
        };

        /*!
         * \brief Process spawns.
         * Takes spawner messages and creates or deletes entities.
         * \param messages Spawner messages from main engine loop.
         * \param world Reference to the entity manager.
         * \return void
         */
        inline void process(const message_container messages, entity_manager& world) {
            for(auto & m_it : messages) {
                if(m_it.get_cmd() == "new") {
                    auto s_it = spawner.find(m_it.get_arg(0));
                    if(s_it != spawner.end())
                        //  Make sure the number of arguments match what's expected.
                        if(m_it.num_args() == s_it->second.first + 1) {
                            entity e_id = world.new_entity();
                            s_it->second.second(e_id, world, m_it.get_arglist());
                        }
                }

                if(m_it.get_cmd() == "delete") {
                    const_component_container<cmp::name> name_components = world.get_components<cmp::name>();

                    //  Check all named entities and delete if it exists.
                    for(auto & c_it : name_components) {
                        if(m_it.get_arg(0) == c_it.second->name_str) world.delete_entity(c_it.first);
                    }
                }
            }  //  End for(m_it)
        };

        /*!
         * \brief Add a spawn to the spawner map.
         * Stores a function in the map for entity creation on demand.
         * \param name Reference name for the spwaner item.
         * \param num_args Number of arguments the spawn accepts.
         * \param func Function for creating the entity.
         * \return True if inserted into the spawn map, false if not.
         */
        inline const bool add_spawn(const std::string name, const std::size_t num_args,
                                    void func(const entity, entity_manager&, const msg_arg_list)) {
            auto ret = spawner.insert(std::make_pair(name, std::make_pair(num_args, func)));
            return ret.second;
        };

    private:
        std::map<std::string, std::pair<std::size_t,
            std::function<void(const entity, entity_manager&, const msg_arg_list)>>> spawner;
};

template <> inline bool spawn_manager::manager<spawn_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
