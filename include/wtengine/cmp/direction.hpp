/*!
 * WTEngine | File:  direction.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
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
         * \brief Create a new direction component.
         */
        direction(void);

        /*!
         * \brief Create a new direction component, set draw rotated flag.
         * \param r Draw rotated flag.
         */
        direction(const bool& r);

        /*!
         * \brief Create a new direction component, set angle.
         * \param a Angle of direction in radians.
         */
        direction(const float& a);

        /*!
         * \brief Create a new direction component, set angle and rotated flag.
         * \param a Angle of direction in radians.
         * \param r Draw rotated flag.
         */
        direction(
            const float& a,
            const bool& r
        );

        /*!
         * \brief Set angle of direction using degrees.
         * \param a Angle in degrees.
         */
        void set_degrees(const float& a);

        /*!
         * \brief Get angle of direction in degrees.
         * \return Angle in degrees.
         */
        const float get_degrees(void) const;

        /*!
         * \brief Set angle of direction in radians.
         * \param a Angle in radians.
         */
        void set_radians(const float& a);

        /*!
         * \brief Get angle of direction in radians.
         * \return Angle in radians.
         */
        const float get_radians(void) const;

        /*!
         * \brief Check if the entity should be drawn rotated.
         * \return True if rotated, false if not.
         */
        const bool show_rotated(void) const;

        /*!
         * \brief Set drawing rotated flag to true.
         */
        void set_rotated(void);

        /*!
         * \brief Set drawing rotated flag to false.
         */
        void unset_rotated(void);

    private:
        float angle;        //  Angle of direction.
        bool draw_rotated;  //  Flag to draw rotated.
};

} //  namespace cmp

} //  namespace wte

#endif
