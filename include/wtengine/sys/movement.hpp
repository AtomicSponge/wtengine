/*!
 * WTEngine | File:  movement.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_SYS_MOVEMENT_HPP
#define WTE_SYS_MOVEMENT_HPP

#include <cmath>

#include "wtenginestem.hpp"

namespace wte
{

namespace sys
{

/*!
 * \class movement
 * \brief Moves entities based on their velocity.
 */
class movement final : public system {
    public:
        /*!
         * \brief Movement System constructor.
         */
        movement();

        /*!
         * \brief Movement System destructor.
         */
        ~movement();

        /*!
         * \brief Movement system run.
         * 
         * All entities with a velocity component will be moved.
         * Also checks entities are within their bounding boxes.
         * 
         * \param world Reference to the entity manager.
         * \param messages Reference to the message manager.
         * \param current_time Current engine time.
         */
        void run(mgr::entity_manager& world,
                 mgr::message_manager& messages,
                 const int64_t& current_time) override;
};

} //  namespace sys

} //  namespace wte

#endif
