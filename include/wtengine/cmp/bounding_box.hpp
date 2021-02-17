/*!
 * WTEngine | File:  bounding_box.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_CMP_BOUNDING_BOX_HPP
#define WTE_CMP_BOUNDING_BOX_HPP

#include "wtengine/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class bounding_box
 * \brief Set a bounding box for an entity.
 * 
 * Entities can not move out of this box.
 */
class bounding_box final : public component {
    public:
        /*!
         * \brief Bounding box constructor.
         * 
         * \param lx Left X
         * \param ly Left Y
         * \param rx Right X
         * \param ry Right Y
         */
        bounding_box(const float lx,
                            const float ly,
                            const float rx,
                            const float ry) :
               min_x(lx), min_y(ly), max_x(rx), max_y(ry) {};

        /*!
         * \brief Set left X of the bounding box.
         * 
         * \param mx Left X
         */
        void set_min_x(const float& mx) {
            min_x = mx;
        };

        /*!
         * \brief Set left Y of the bounding box.
         * 
         * \param my Left Y
         */
        void set_min_y(const float& my) {
            min_y = my;
        };

        /*!
         * \brief Set right X of the bounding box.
         * 
         * \param mx Right X
         */
        void set_max_x(const float& mx) {
            max_x = mx;
        };

        /*!
         * \brief Set right Y of the bounding box.
         * 
         * \param my right Y
         */
        void set_max_y(const float& my) {
            max_y = my;
        };

        /*!
         * \brief Get the left X of the bounding box.
         * 
         * \return Left X
         */
        const float get_min_x(void) const {
            return min_x;
        };

        /*!
         * \brief Get the left Y of the bounding box.
         * 
         * \return Left Y
         */
        const float get_min_y(void) const {
            return min_y;
        };

        /*!
         * \brief Get the right X of the bounding box.
         * 
         * \return Right X
         */
        const float get_max_x(void) const {
            return max_x;
        };

        /*!
         * \brief Get the right Y of the bounding box.
         * 
         * \return Right Y
         */
        const float get_max_y(void) const {
            return max_y;
        };

    private:
        float min_x, min_y;
        float max_x, max_y;
};

} //  namespace cmp

} //  namespace wte

#endif
