/*!
 * \brief WTEngine | File:  movement.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Movement.
 */

#ifndef WTE_SYS_MOVEMENT_HPP
#define WTE_SYS_MOVEMENT_HPP

#include <cmath>

#include "system.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class movement
 * \brief Moves entities.
 */
class movement final : public system {
    public:
        /*!
         * Movement System constructor.
         * \param void
         * \return void
         */
        inline movement() : system("movement") {};

        /*!
         * Movement System destructor.
         * \param void
         * \return void
         */
        inline ~movement() {};

        /*!
         * \brief Movement system run.
         * All entities with a velocity component will be moved.
         * Also checks entities are within their bounding boxes.
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         * \return void
         */
        inline void run(mgr::entity_manager& world,
                        mgr::message_manager& messages,
                        const int64_t& current_time) override {
            //  Find the entities with a velocity component.
            const_component_container<cmp::velocity> vel_components = world.get_components<cmp::velocity>();
            for(auto & it : vel_components) {
                if(world.has_component<cmp::location>(it.first) &&
                   world.has_component<cmp::direction>(it.first))
                {
                    world.set_component<cmp::location>(it.first)->adjust_x(
                        it.second->get_x_vel() *
                        std::cos(world.get_component<cmp::direction>(it.first)->get_radians())
                    );

                    world.set_component<cmp::location>(it.first)->adjust_y(
                        it.second->get_y_vel() *
                        std::sin(world.get_component<cmp::direction>(it.first)->get_radians())
                    );
                }
            }

            //  Now check all bounding boxes.
            const_component_container<cmp::bounding_box> bbox_components = world.get_components<cmp::bounding_box>();
            for(auto & it : bbox_components) {
                if(world.has_component<cmp::location>(it.first)) {
                    if(world.get_component<cmp::location>(it.first)->get_x() < it.second->get_min_x())
                        world.set_component<cmp::location>(it.first)->set_x(it.second->get_min_x());
                    else if(world.get_component<cmp::location>(it.first)->get_x() > it.second->get_max_x())
                        world.set_component<cmp::location>(it.first)->set_x(it.second->get_max_x());

                    if(world.get_component<cmp::location>(it.first)->get_y() < it.second->get_min_y())
                        world.set_component<cmp::location>(it.first)->set_y(it.second->get_min_y());
                    else if(world.get_component<cmp::location>(it.first)->get_y() > it.second->get_max_y())
                        world.set_component<cmp::location>(it.first)->set_y(it.second->get_max_y());
                }
            }
        };
};

} //  namespace sys

} //  namespace wte

#endif
