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

#define _USE_MATH_DEFINES

#include <cmath>

#include "component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Direction component
 * Store the direction of travel for an entity
 */
class direction final : public component {
    public:
        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \return void
         */
        inline direction(void) : angle(0.0f), draw_rotated(true) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \return void
         */
        inline direction(const bool r) : angle(0.0f), draw_rotated(r) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \return void
         */
        inline direction(const float a) : angle(a), draw_rotated(true) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \param r Draw rotated flag.
         * \return void
         */
        inline direction(const float a, const bool r) : angle(a), draw_rotated(r) {};

        /*!
         * Set angle of direction.
         * \param a Angle in degrees.
         * \return void
         */
        inline void set_degrees(const float& a) {
            angle = a * M_PI / 180.0f;
        };

        /*!
         * Get angle of direction.
         * \param void
         * \return Angle in degrees.
         */
        inline const float get_degrees(void) const {
            return (angle * 180.0f / M_PI);
        };

        /*!
         * Set angle of direction.
         * \param a Angle in degrees.
         * \return void
         */
        inline void set_radians(const float& a) {
            angle = a;
        };

        /*!
         * Get angle of direction in radians.
         * \param void
         * \return Angle in radians.
         */
        inline const float get_radians(void) const {
            return angle;
        };

        /*!
         * Check if the entity should be drawn rotated.
         * \param void
         * \return True if rotated, false if not.
         */
        inline const bool show_rotated(void) const {
            return draw_rotated;
        };

        /*!
         * \brief Set drawing rotated.
         * Sets internal flag to true.
         * \param void
         * \return void
         */
        inline void set_rotated(void) {
            draw_rotated = true;
        };

        /*!
         * \brief Unset drawing rotated.
         * Sets internal flag to false.
         * \param void
         * \return void
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
