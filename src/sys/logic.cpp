/*!
 * WTEngine | File:  logic.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
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
logic::~logic() {};

/*
 *
 */
void logic::run(void) {
    //  Find the entities with the input handler component
    component_container<cmp::ai> ai_components = mgr::entities::set_components<cmp::ai>();

    for(auto & it : ai_components) {
        try {
            //  Process enabled or disabled ai
            if(mgr::entities::get_component<cmp::enabled>(it.first)->check())
                it.second->run_enabled(it.first);
            else
                it.second->run_disabled(it.first);
        } catch(const wte_exception& e) { alert::set_alert(e.what()); }
    }
}

} //  namespace sys

} //  namespace wte
