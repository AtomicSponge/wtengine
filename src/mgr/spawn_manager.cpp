/*!
 * WTEngine | File:  spawn_manager.cpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/spawn_manager.hpp"

namespace wte
{

namespace mgr
{

template <> bool spawn_manager::manager<spawn_manager>::initialized = false;

spawn_manager::spawn_manager() {
    spawner.clear();
}

spawn_manager::~spawn_manager() {
    spawner.clear();
}

void spawn_manager::process(const message_container& messages, entity_manager& world) {
    for(auto & m_it : messages) {
        if(m_it.get_cmd() == "new") {
            auto s_it = spawner.find(m_it.get_arg(0));
            if(s_it != spawner.end())
                //  Make sure the number of arguments match what's expected.
                //  Note that we do not count the first argument.
                if(m_it.num_args() == s_it->second.first + 1) {
                    entity_id e_id = world.new_entity();
                    s_it->second.second(e_id, world, m_it.get_arglist());
                }
        }

        if(m_it.get_cmd() == "delete") {
            entity_id delete_entity_id = world.get_id(m_it.get_arg(0));
            if(delete_entity_id != WTE_ENTITY_ERROR) {
                world.delete_entity(delete_entity_id);
            }
        }
    }  //  End for(m_it)
}

const bool spawn_manager::add_spawn(const std::string& name, const std::size_t& num_args,
                            void func(const entity_id&, entity_manager&, const msg_arg_list&)) {
    auto ret = spawner.insert(std::make_pair(name, std::make_pair(num_args, func)));
    return ret.second;
}

} //  namespace mgr

} //  namespace wte
