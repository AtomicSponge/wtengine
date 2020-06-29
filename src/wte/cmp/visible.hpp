/*!
 * \brief WTEngine | File:  visible.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Visible component.
 */

#ifndef WTE_CMP_VISIBLE_HPP
#define WTE_CMP_VISIBLE_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class Visible component
 * Toggles if an entity will be drawn by the renderer
 */
class visible final : public component {
    public:
        /*!
         * \brief Visible constructor.
         * Call this to be visible by default.
         * \param void
         * \return void
         */
        inline visible() : is_visible(true) {};

        /*!
         * \brief Visible constructor.
         * Call this to set if the entity is visible.
         * \param v Boolean value for visibility.
         * \return void
         */
        inline visible(const bool v) : is_visible(v) {};

        /*!
         *
         */
        inline const bool check(void) const {
            return is_visible;
        };

        /*!
         *
         */
        inline void show(void) {
            is_visible = true;
        };

        /*!
         *
         */
        inline void hide(void) {
            is_visible = false;
        };

    private:
        bool is_visible;
};

} //  namespace cmp

} //  namespace wte

#endif
