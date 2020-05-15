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
#include <map>
#include <functional>

#include "manager.hpp"
#include "entity_manager.hpp"
#include "../message.hpp"

namespace wte
{

namespace mgr
{

//! spawn_manager class
/*!
 * Spawn stuff
 */
class spawn_manager final : public manager<spawn_manager> {
    public:
        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline spawn_manager() { spawner.clear(); };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline ~spawn_manager() { spawner.clear(); };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline void process(const message_container messages, entity_manager& world) {
            for(auto it = messages.begin(); it != messages.end(); it++) {
                if(it->get_cmd() == "new") {
                    auto s_it = spawner.find(it->get_arg(0));
                    if(s_it != spawner.end()) {
                        //s_it.second;
                    }
                }

                if(it->get_cmd() == "delete") {
                    component_container name_components = world.get_components<cmp::name>();
                
                    //  Check all named entities and delete if it exists.
                    for(component_iterator c_it = name_components.begin(); c_it != name_components.end(); c_it++) {
                        if(it->get_arg(0) == dynamic_cast<cmp::name*>(c_it->second.get())->name_str) {
                            world.delete_entity(c_it->first);
                        }
                    }
                }
            }
        };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline void add_spawn(std::string name, void func()) {
            spawner.insert(std::make_pair(name, func));
        };

    private:
        std::map<std::string, std::function<void()>> spawner;
};

template <> inline bool spawn_manager::manager<spawn_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
