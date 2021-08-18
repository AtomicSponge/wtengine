/*!
 * WTEngine | File:  animate.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
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
void animate::run(void) {
    component_container<cmp::gfx> animation_components = mgr::world::set_components<cmp::gfx>();

    for(auto& it: animation_components)
        if(it.second->is_visible()) try { it.second->run(it.first); } catch(...) { throw; }
}

} //  namespace sys

} //  namespace wte
