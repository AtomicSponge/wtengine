/*!
 * WTEngine | File:  system_manager.cpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include "wtengine/mgr/system_manager.hpp"

namespace wte
{

namespace mgr
{

template <> bool system_manager::manager<system_manager>::initialized = false;

system_manager::system_manager() : finalized(false) {
    systems.clear();
}

system_manager::~system_manager() {
    systems.clear();
}

void system_manager::clear(void) {
    systems.clear();
    finalized = false;
}

void system_manager::finalize(void) {
    finalized = true;
}

bool system_manager::empty(void) {
    return systems.empty();
}

bool system_manager::add(sys::system_uptr new_system) {
    if(finalized == true) return false;

    for(auto & it : systems)
        if((it)->get_name() == new_system->get_name()) return false;

    systems.push_back(std::move(new_system));
    return true;
}

void system_manager::run(mgr::entity_manager& world,
                mgr::message_manager& messages,
                const int64_t& current_time) {
    for(auto & it : systems)
        if((it)->is_enabled()) (it)->run(world, messages, current_time);
}

void system_manager::dispatch(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) {
    component_container<cmp::dispatcher> dispatch_components =
        world.set_components<cmp::dispatcher>();

    while(true) {
        message_container temp_msgs = messages.get_messages("entities");
        if(temp_msgs.empty()) break;  //  No messages, end while(true) loop.

        for(auto & c_it : dispatch_components) {
            for(auto m_it = temp_msgs.begin(); m_it != temp_msgs.end();) {
                if(m_it->get_to() == world.get_name(c_it.first)) {
                    c_it.second->proc_msg(c_it.first, *m_it, world, messages, current_time);
                    m_it = temp_msgs.erase(m_it);
                } else m_it++;
            }
            if(temp_msgs.empty()) break;  //  No messages left, end comp loop.
        }
    }
}

void system_manager::reload_temp_bitmaps(mgr::entity_manager& world) {
    component_container<cmp::background> background_components =
        world.set_components<cmp::background>();

    for(auto & it : background_components) {
        it.second.get()->reload_background_bitmap();
    }

    component_container<cmp::overlay> overlay_components =
        world.set_components<cmp::overlay>();

    for(auto & it : overlay_components) {
        it.second.get()->reload_overlay_bitmap();
    }
}

const bool system_manager::enable_system(const std::string& sys) {
    for(auto & it : systems) {
        if((it)->get_name() == sys) (it)->enable();
        return true;
    }
    return false;
}

const bool system_manager::disable_system(const std::string& sys) {
    for(auto & it : systems) {
        if((it)->get_name() == sys) (it)->disable();
        return true;
    }
    return false;
}

} //  namespace mgr

} //  namespace wte
