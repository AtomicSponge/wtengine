/*!
 * \brief WTEngine | File:  animate.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Animation system.  Updates all components tagged with an animator.
 */

#ifndef WTE_SYS_ANIMATE_HPP
#define WTE_SYS_ANIMATE_HPP

#include "system.hpp"

namespace wte
{

namespace sys
{

//! Animate system
/*!
 * Animate
 */
class animate : public system {
    public:
        inline animate() : system("animate") {};
        inline ~animate() {};

        //! Animate system run member
        inline void run(mgr::entity_manager&, mgr::message_manager&, int64_t);
};

/*!
 * Animate system run
 */
inline void animate::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    component_container animation_components;

    animation_components = world.get_components<cmp::animator>();

    for(component_iterator it = animation_components.begin(); it != animation_components.end(); it++) {
        dynamic_cast<cmp::animator*>(it->second.get())->run(it->first, world, current_time);
    }
}

} //  namespace sys

} //  namespace wte

#endif
