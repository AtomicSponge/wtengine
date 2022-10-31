/*!
 * wtengine | File:  animate.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/sys/animate.hpp"

namespace wte::sys::gfx {

/*
 *
 */
animate::animate() : system("animate") {}

/*
 *
 */
void animate::run(void) {
    component_container<cmp::gfx::gfx> animation_components =
        mgr::world::set_components<cmp::gfx::gfx>();

    for(auto& it: animation_components)
        try {
            if(it.second->visible) it.second->animate(it.first);
        } catch(...) { throw; }
}

}  //  end namespace wte::sys
