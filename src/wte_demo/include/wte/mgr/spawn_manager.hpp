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
            for(auto m_it = messages.begin(); m_it != messages.end(); m_it++) {
                if(m_it->get_cmd() == "new") {
                    auto s_it = spawner.find(m_it->get_arg(0));
                    if(s_it != spawner.end())
                        //  Make sure the number of arguments match what's expected.
                        if(m_it->num_args() == s_it->second.first + 1) {
                            entity e_id = world.new_entity();
                            s_it->second.second(e_id, world, m_it->get_arglist());
                        }
                }

                if(m_it->get_cmd() == "delete") {
                    component_container name_components = world.get_components<cmp::name>();

                    //  Check all named entities and delete if it exists.
                    for(component_iterator c_it = name_components.begin(); c_it != name_components.end(); c_it++) {
                        if(m_it->get_arg(0) == dynamic_cast<cmp::name*>(c_it->second.get())->name_str)
                            world.delete_entity(c_it->first);
                    }
                }
            }  //  End for(m_it)
        };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline void add_spawn(std::string name, std::size_t num_args, void func(entity, entity_manager&, msg_arg_list)) {
            spawner.insert(std::make_pair(name, std::make_pair(num_args, func)));
        };

    private:
        std::map<std::string, std::pair<std::size_t, std::function<void(entity, entity_manager&, msg_arg_list)>>> spawner;
};

template <> inline bool spawn_manager::manager<spawn_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
