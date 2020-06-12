/*!
 * \brief WTEngine | File:  direction.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Direction component.
 */

#ifndef WTE_CMP_DIRECTION_HPP
#define WTE_CMP_DIRECTION_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Direction component
/*!
 * Store the direction of travel for an entity
 */
class direction final : public component {
    public:
        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \return void
         */
        inline direction(float a) : angle(a), draw_rotated(true) {};

        float angle;
        bool draw_rotated;
};

} //  namespace cmp

} //  namespace wte

#endif
