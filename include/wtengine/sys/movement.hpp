/*!
 * WTEngine | File:  movement.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_SYS_MOVEMENT_HPP
#define WTE_SYS_MOVEMENT_HPP

#include <cmath>

#include "wtengine/sys/system.hpp"

namespace wte::sys {

/*!
 * \class movement
 * \brief Moves entities based on their velocity.
 */
class movement final : public system {
    public:
        movement();
        ~movement() = default;

        /*!
         * \brief All entities with a velocity component will be moved.
         * 
         * Also checks entities are within their bounding boxes.
         */
        void run(void) override;
};

}  //  namespace wte::sys

#endif
