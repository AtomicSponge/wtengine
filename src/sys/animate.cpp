/*!
 * WTEngine | File:  animate.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/sys/animate.hpp"

namespace wte
{

namespace sys
{

/*
 *
 */
animate::animate() : system("animate") {}

/*
 *
 */
animate::~animate() {}

/*
 *
 */
void animate::run(void) {
    component_container<cmp::animator> animation_components = mgr::entities::set_components<cmp::animator>();

    for(auto & it : animation_components) {
        try {
            if(mgr::entities::get_component<cmp::visible>(it.first)->check())
                it.second->run(it.first);
        } catch(const wte_exception& e) { alert::set(e.what()); }
    }
}

} //  namespace sys

} //  namespace wte
