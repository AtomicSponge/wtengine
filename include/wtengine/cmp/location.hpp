/*!
 * WTEngine | File:  location.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_LOCATION_HPP
#define WTE_CMP_LOCATION_HPP

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class location
 * \brief Store the x/y location of an entity in the arena.
 */
class location final : public component {
    public:
        /*!
         * \brief Location constructor.
         * 
         * \param x Horizontal location of the entity.
         * \param y Verticle location of the entity.
         */
        location(float x, float y) : pos_x(x), pos_y(y) {};

        /*!
         * \brief Get X location.
         * 
         * \return X location of entity.
         */
        const float get_x(void) const {
            return pos_x;
        };

        /*!
         * \brief Get Y location.
         * 
         * \return Y location of entity.
         */
        const float get_y(void) const {
            return pos_y;
        };

        /*!
         * \brief Set X location.
         * 
         * \param x New X location for entity.
         */
        void set_x(const float& x) {
            pos_x = x;
        };

        /*!
         * \brief Set Y location.
         * 
         * \param y New Y location for entity.
         */
        void set_y(const float& y) {
            pos_y = y;
        };

        /*!
         * \brief Adjust X location.
         * 
         * \param x X offset for entity.
         */
        void adjust_x(const float& x) {
            pos_x += x;
        };

        /*!
         * \brief Adjust Y location.
         * 
         * \param y Y offset for entity.
         */
        void adjust_y(const float& y) {
            pos_y += y;
        };

    private:
        float pos_x, pos_y;
};

} //  namespace cmp

} //  namespace wte

#endif
