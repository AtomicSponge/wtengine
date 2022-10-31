/*!
 * wtengine | File:  spawner.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/spawner.hpp"

namespace wte::mgr {

template <> bool manager<spawner>::initialized = false;

std::map<
    const std::string,
    std::pair<
        const std::size_t,
        const std::function<void(const entity_id&, const msg_args&)>
>> spawner::spawns;

/*
 *
 */
bool spawner::add(
    const std::string& name,
    const std::size_t& num_args,
    const std::function<void(const entity_id&, const msg_args&)>& func
) {
    auto ret = spawns.insert(std::make_pair(name, std::make_pair(num_args, func)));
    return ret.second;
}

/*
 *
 */
bool spawner::remove(const std::string& name) {
    auto it = std::find_if(spawns.begin(), spawns.end(),
                           [&name](const auto& e){ return e.first == name; });
    if(it != spawns.end()) {
        spawns.erase(it);
        return true;
    }
    return false;
}

/*
 *
 */
bool spawner::spawn(const std::string& name, const msg_args& args) {
    auto it = spawns.find(name);
    if(it != spawns.end()) {
        if(args.size() == it->second.first) {
            try {
                it->second.second(mgr::world::new_entity(), args);
            } catch(const std::exception& e) { throw e; }
            return true;
        }
    }
    return false;
}

/*
 *
 */
void spawner::process_messages(const message_container& messages) {
    for(auto& m_it: messages) {
        if(m_it.get_cmd() == "new") {
            auto s_it = spawns.find(m_it.get_arg(0));
            if(s_it != spawns.end())
                //  Make sure the number of arguments match what's expected.
                //  Note that we do not count the first argument.
                if(m_it.num_args() == s_it->second.first + 1) {
                    try {
                        s_it->second.second(mgr::world::new_entity(), m_it.get_args());
                    } catch(const std::exception& e) { throw e; }
                }
        }

        if(m_it.get_cmd() == "delete") {
            entity_id delete_entity_id = mgr::world::get_id(m_it.get_arg(0));
            if(delete_entity_id != mgr::world::ENTITY_ERROR) {
                mgr::world::delete_entity(delete_entity_id);
            }
        }
    }  //  End for(m_it)
}

}  //  end namespace wte::mgr
