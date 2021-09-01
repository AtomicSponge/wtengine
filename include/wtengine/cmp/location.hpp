/*!
 * WTEngine | File:  location.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_LOCATION_HPP
#define WTE_CMP_LOCATION_HPP

#include "wtengine/cmp/component.hpp"

namespace wte::cmp {

/*!
 * \class location
 * \brief Store the x/y location of an entity in the arena.
 */
class location final : public component {
    public:
        /*!
         * \brief Create a new Location component.
         * \param x Horizontal location of the entity.
         * \param y Verticle location of the entity.
         */
        location(
            const float& x,
            const float& y
        );

        ~location() = default;  //!<  Default destructor.

        /*!
         * \brief Get X location.
         * \return X location of entity.
         */
        const float get_x(void) const;

        /*!
         * \brief Get Y location.
         * \return Y location of entity.
         */
        const float get_y(void) const;

        /*!
         * \brief Set X location.
         * \param x New X location for entity.
         */
        void set_x(const float& x);

        /*!
         * \brief Set Y location.
         * \param y New Y location for entity.
         */
        void set_y(const float& y);

        /*!
         * \brief Adjust X location.
         * \param x X offset for entity.
         */
        void adjust_x(const float& x);

        /*!
         * \brief Adjust Y location.
         * \param y Y offset for entity.
         */
        void adjust_y(const float& y);

    private:
        float pos_x, pos_y; //  X & Y location.
};

} //  namespace wte::cmp

#endif
