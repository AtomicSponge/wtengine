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

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <limits>

#include "component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class direction
 * \brief Store the direction of travel for an entity in radians.
 * Can also convert to degrees.
 */
class direction final : public component {
    public:
        /*!
         * Direction constructor.
         */
        inline direction(void) : angle(0.0f), draw_rotated(true) {};

        /*!
         * Direction constructor.
         * \param r Draw rotated flag.
         */
        inline direction(const bool r) : angle(0.0f), draw_rotated(r) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction in radians.
         */
        inline direction(const float a) : angle(a), draw_rotated(true) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction in radians.
         * \param r Draw rotated flag.
         */
        inline direction(const float a, const bool r) : angle(a), draw_rotated(r) {};

        /*!
         * Set angle of direction using degrees.
         * \param a Angle in degrees.
         */
        inline void set_degrees(const float& a) {
            angle = a * M_PI / 180.0f;
        };

        /*!
         * Get angle of direction in degrees.
         * \return Angle in degrees.
         */
        inline const float get_degrees(void) const {
            return (angle * 180.0f / M_PI);
        };

        /*!
         * Set angle of direction in radians.
         * \param a Angle in radians.
         */
        inline void set_radians(const float& a) {
            angle = a;
        };

        /*!
         * Get angle of direction in radians.
         * \return Angle in radians.
         */
        inline const float get_radians(void) const {
            return angle;
        };

        /*!
         * Check if the entity should be drawn rotated.
         * \return True if rotated, false if not.
         */
        inline const bool show_rotated(void) const {
            return draw_rotated;
        };

        /*!
         * \brief Set drawing rotated.
         * Sets internal flag to true.
         */
        inline void set_rotated(void) {
            draw_rotated = true;
        };

        /*!
         * \brief Unset drawing rotated.
         * Sets internal flag to false.
         */
        inline void unset_rotated(void) {
            draw_rotated = false;
        };

    private:
        float angle;
        bool draw_rotated;
};

} //  namespace cmp

} //  namespace wte

#endif
