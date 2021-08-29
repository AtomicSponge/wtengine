/*!
 * WTEngine | File:  systems.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/systems.hpp"

namespace wte {

namespace mgr {

template <> bool systems::manager<systems>::initialized = false;

std::vector<sys::system_uptr> systems::_systems;
bool systems::finalized = false;

/*
 *
 */
void systems::clear(void) {
    _systems.clear();
    finalized = false;
}

/*
 *
 */
void systems::finalize(void) { finalized = true; }

/*
 *
 */
const bool systems::empty(void) { return _systems.empty(); }

/*
 *
 */
const bool systems::add(sys::system_uptr new_system) {
    if(finalized == true) return false;

    for(auto& it: _systems)
        if((it)->get_name() == new_system->get_name()) return false;

    _systems.push_back(std::move(new_system));
    return true;
}

/*
 *
 */
void systems::run() {
    for(auto& it: _systems)
        try { (it)->run(); } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
}

/*
 *
 */
void systems::dispatch(void) {
    component_container<cmp::dispatcher> dispatch_components =
        mgr::world::set_components<cmp::dispatcher>();

    while(true) {  //  Infinite loop to verify all current messages are processed.
        message_container temp_msgs = mgr::messages::get_messages("entities");
        if(temp_msgs.empty()) break;  //  No messages, end while(true) loop.

        //  For all messages, check each dispatch component.
        for(auto& m_it: temp_msgs) { for(auto& c_it: dispatch_components) {
            try {
                if(m_it.get_to() == mgr::world::get_name(c_it.first)) {
                    c_it.second->proc_msg(c_it.first, m_it);
                    break;  //  Found, stop checking dispatch components.
                }
            } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
        }}
    }
}

} //  namespace mgr

} //  namespace wte
