/*!
 * WTEngine | File:  direction.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_DIRECTION_HPP
#define WTE_CMP_DIRECTION_HPP

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <cmath>
#include <limits>

#include "wtengine/cmp/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class direction
 * \brief Store the direction of travel for an entity in radians.
 * 
 * Can also convert to degrees.
 */
class direction final : public component {
    public:
        /*!
         * \brief Direction constructor.
         */
        direction(void);

        /*!
         * \brief Direction constructor.
         * 
         * \param r Draw rotated flag.
         */
        direction(const bool r);

        /*!
         * \brief Direction constructor.
         * 
         * \param a Angle of direction in radians.
         */
        direction(const float a);

        /*!
         * \brief Direction constructor.
         * 
         * \param a Angle of direction in radians.
         * \param r Draw rotated flag.
         */
        direction(const float a, const bool r);

        /*!
         * \brief Set angle of direction using degrees.
         * 
         * \param a Angle in degrees.
         */
        void set_degrees(const float& a);

        /*!
         * \brief Get angle of direction in degrees.
         * 
         * \return Angle in degrees.
         */
        const float get_degrees(void) const;

        /*!
         * \brief Set angle of direction in radians.
         * 
         * \param a Angle in radians.
         */
        void set_radians(const float& a);

        /*!
         * \brief Get angle of direction in radians.
         * 
         * \return Angle in radians.
         */
        const float get_radians(void) const;

        /*!
         * \brief Check if the entity should be drawn rotated.
         * 
         * \return True if rotated, false if not.
         */
        const bool show_rotated(void) const;

        /*!
         * \brief Set drawing rotated.
         * 
         * Sets internal flag to true.
         */
        void set_rotated(void);

        /*!
         * \brief Unset drawing rotated.
         * 
         * Sets internal flag to false.
         */
        void unset_rotated(void);

    private:
        float angle;
        bool draw_rotated;
};

} //  namespace cmp

} //  namespace wte

#endif
