/*!
 * WTEngine | File:  logic.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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
    component_container<cmp::ai> ai_components = mgr::world::set_components<cmp::ai>();

    //  Process enabled or disabled ai
    for(auto& it: ai_components) {
        try {
            if(it.second->enabled) it.second->run_enabled(it.first);
            else it.second->run_disabled(it.first);
        } catch(...) { throw; }
    }
}

}  //  end namespace wte::sys
