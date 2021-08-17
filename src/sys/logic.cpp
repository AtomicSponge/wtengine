/*!
 * WTEngine | File:  logic.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/logic.hpp"

namespace wte
{

namespace sys
{

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

    for(auto& it: ai_components) {
        try {
            //  Process enabled or disabled ai
            if(it.second->status())
                it.second->run_enabled(it.first);
            else
                it.second->run_disabled(it.first);
        } catch(...) { throw; }
    }
}

} //  namespace sys

} //  namespace wte
