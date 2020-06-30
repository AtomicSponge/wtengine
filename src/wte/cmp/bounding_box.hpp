/*!
 * \brief WTEngine | File:  bounding_box.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Bounding box component.
 */

#ifndef WTE_CMP_BOUNDING_BOX_HPP
#define WTE_CMP_BOUNDING_BOX_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Bounding Box component
 * Set a bounding box for an entity.
 * Entities can not move out of this box.
 */
class bounding_box final : public component {
    public:
        /*!
         * \brief Bounding box constructor.
         * \param lx Left X
         * \param ly Left Y
         * \param rx Right X
         * \param ry Right Y
         * \return void
         */
        inline bounding_box(const float lx,
                            const float ly,
                            const float rx,
                            const float ry) :
               min_x(lx), min_y(ly), max_x(rx), max_y(ry) {};

        /*!
         * Get the left X of the bounding box.
         * \param void
         * \return Left X
         */
        inline const float get_min_x(void) const {
            return min_x;
        };

        /*!
         * Get the left Y of the bounding box.
         * \param void
         * \return Left Y
         */
        inline const float get_min_y(void) const {
            return min_y;
        };

        /*!
         * Get the right X of the bounding box.
         * \param void
         * \return Right X
         */
        inline const float get_max_x(void) const {
            return max_x;
        };

        /*!
         * Get the right Y of the bounding box.
         * \param void
         * \return Right Y
         */
        inline const float get_max_y(void) const {
            return max_y;
        };

    private:
        float min_x, min_y;
        float max_x, max_y;
};

} //  namespace cmp

} //  namespace wte

#endif
