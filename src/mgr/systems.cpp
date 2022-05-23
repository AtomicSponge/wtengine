/*!
 * WTEngine | File:  systems.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/systems.hpp"

namespace wte::mgr {

template <> bool manager<systems>::initialized = false;

std::vector<sys::system_uptr> systems::_systems_timed;
std::vector<sys::system_uptr> systems::_systems_untimed;
bool systems::finalized = false;

/*
 *
 */
void systems::clear(void) {
    _systems_timed.clear();
    _systems_untimed.clear();
    finalized = false;
}

/*
 *
 */
const bool systems::empty(void) { return (_systems_timed.empty() && _systems_untimed.empty()); }

/*
 *
 */
const bool systems::add(sys::system_uptr new_system) {
    if(finalized == true) return false;

    if(new_system->timed) {
        for(auto& it: _systems_timed)
            if((it)->name == new_system->name) return false;
        _systems_timed.push_back(std::move(new_system));
    } else {
        for(auto& it: _systems_untimed)
            if((it)->name == new_system->name) return false;
        _systems_untimed.push_back(std::move(new_system));
    }
    return true;
}

/*
 *
 */
void systems::run() {
    for(auto& it: _systems_timed)
        try { 
            (it)->run();
        } catch(const wte_exception& e) {
            alert::set(e.what(), e.where(), e.when());
        }
}

/*
 *
 */
void systems::run_untimed() {
    for(auto& it: _systems_untimed)
        try { 
            (it)->run();
        } catch(const wte_exception& e) {
            alert::set(e.what(), e.where(), e.when());
        }
}

}  //  end namespace wte::mgr
