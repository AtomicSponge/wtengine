/*!
 * wtengine | File:  systems.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/systems.hpp"

namespace wte::mgr {

template <> bool manager<systems>::initialized = false;

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
const bool systems::empty(void) { return (_systems.empty()); }

/*
 *
 */
void systems::run() {
    for(auto& it: _systems)
        try { 
            (it)->run();
        } catch(const std::exception& e) { throw e; }
}

}  //  end namespace wte::mgr
