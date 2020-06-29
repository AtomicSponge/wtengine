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
        inline direction(const float a) : angle(a), draw_rotated(true) {};

        /*!
         * Direction constructor.
         * \param a Angle of direction.
         * \param r Draw rotated flag.
         * \return void
         */
        inline direction(const float a, const bool r) : angle(a), draw_rotated(r) {};

        /*!
         *
         */
        inline const float get_angle(void) const {
            return angle;
        };

        /*!
         *
         */
        inline void set_angle(const float& a) {
            angle = a;
        };

        /*!
         *
         */
        inline const bool show_rotated(void) const {
            return draw_rotated;
        };

        /*!
         *
         */
        inline void set_rotated(void) {
            draw_rotated = true;
        };

        /*!
         *
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
