/*!
 * WTEngine | File:  spawner.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/spawner.hpp"

namespace wte
{

namespace mgr
{

template <> bool spawner::manager<spawner>::initialized = false;

std::map<std::string, std::pair<std::size_t,
    std::function<void(const entity_id&, const msg_args&)>
>> spawner::spawns = {};

/*
 *
 */
void spawner::process_messages(const message_container& messages) {
    for(auto & m_it : messages) {
        if(m_it.get_cmd() == "new") {
            auto s_it = spawns.find(m_it.get_arg(0));
            if(s_it != spawns.end())
                //  Make sure the number of arguments match what's expected.
                //  Note that we do not count the first argument.
                if(m_it.num_args() == s_it->second.first + 1) {
                    entity_id e_id = mgr::world::new_entity();
                    try {
                        s_it->second.second(e_id, m_it.get_args());
                    } catch(...) { 
                        alert::set("Error spawning entity " + m_it.get_arg(0), "spawner", engine_time::check_time(), true);
                    }
                }
        }

        if(m_it.get_cmd() == "delete") {
            entity_id delete_entity_id = mgr::world::get_id(m_it.get_arg(0));
            if(delete_entity_id != WTE_ENTITY_ERROR) {
                mgr::world::delete_entity(delete_entity_id);
            }
        }
    }  //  End for(m_it)
}

/*
 *
 */
const bool spawner::add_spawn(const std::string& name, const std::size_t& num_args,
                              const std::function<void(const entity_id&, const msg_args&)>& func) {
    auto ret = spawns.insert(std::make_pair(name, std::make_pair(num_args, func)));
    return ret.second;
}

} //  namespace mgr

} //  namespace wte
