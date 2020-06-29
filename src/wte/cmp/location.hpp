/*!
 * \brief WTEngine | File:  location.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Location component.
 */

#ifndef WTE_CMP_LOCATION_HPP
#define WTE_CMP_LOCATION_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Location component
 * Store the x/y location of an entity
 */
class location final : public component {
    public:
        /*!
         * Location constructor.
         * \param x Horizontal location of the entity.
         * \param y Verticle location of the entity.
         * \return void
         */
        inline location(float x, float y) : pos_x(x), pos_y(y) {};

        /*!
         *
         */
        inline const float get_x(void) const {
            return pos_x;
        };

        /*!
         *
         */
        inline const float get_y(void) const {
            return pos_y;
        };

        /*!
         *
         */
        inline void set_x(const float& x) {
            pos_x = x;
        };

        /*!
         *
         */
        inline void set_y(const float& y) {
            pos_y = y;
        };

        /*!
         *
         */
        inline void adjust_x(const float& x) {
            pos_x += x;
        };

        /*!
         *
         */
        inline void adjust_y(const float& y) {
            pos_y += y;
        };

    private:
        float pos_x, pos_y;
};

} //  namespace cmp

} //  namespace wte

#endif
