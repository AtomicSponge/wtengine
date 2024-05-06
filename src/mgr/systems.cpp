/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
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
bool systems::empty(void) { return (_systems.empty()); }

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
