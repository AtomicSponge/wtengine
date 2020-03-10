/*!
 * \brief WTEngine | File:  animate.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Animation system.  Updates all sprite components.
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
  WIP
*/
class animate : public system {
    public:
        inline animate() : system("animate") {};
        inline ~animate() {};

        //! Animate system run member
        inline void run(mgr::entity_manager&, mgr::message_manager&, int64_t);
};

/*!
  WIP
*/
inline void animate::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    component_container sprite_components;

    sprite_components = world.get_components<cmp::sprite>();
}

} //  namespace sys

} //  namespace wte

#endif
