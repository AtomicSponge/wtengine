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

/*!
 * \class animate
 * \brief Find the animate components and process them.
 */
class animate final : public system {
    public:
        inline animate() : system("animate") {};
        inline ~animate() {};

        /*!
         * \brief Animate system run.
         * Gets all animation components and processes their run members.
         * The entity must also have the visible component and is set visible to be drawn.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) override {
            component_container<cmp::animator> animation_components = world.set_components<cmp::animator>();

            for(auto & it : animation_components) {
                if(world.has_component<cmp::visible>(it.first) &&
                   world.get_component<cmp::visible>(it.first)->check())
                        it.second->run(it.first, world, current_time);
            }
        }
};

} //  namespace sys

} //  namespace wte

#endif
