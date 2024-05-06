/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/sys/logic.hpp"

namespace wte::sys {

/*
 *
 */
logic::logic() : system("logic") {};

/*
 *
 */
void logic::run(void) {
  //  Find the entities with the input handler component
  component_container<cmp::ai> ai_components =
    mgr::world::set_components<cmp::ai>();

  //  Process enabled or disabled ai
  for(auto& it: ai_components) {
    (it.second->enabled ?
      it.second->enabled_ai(it.first) :
      it.second->disabled_ai(it.first));
  }
}

}  //  end namespace wte::sys
