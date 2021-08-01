/*!
 * WTEngine | File:  visible.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CMP_VISIBLE_HPP
#define WTE_CMP_VISIBLE_HPP

#include "wtengine/cmp/component.hpp"

namespace wte
{

namespace cmp
{

/*!
 * \class visible
 * \brief Toggles if an entity will be drawn by the renderer.
 */
class visible final : public component {
    public:
        /*!
         * \brief Create a new Visible component.
         */
        visible();

        /*!
         * \brief Create a new Visible component and set the visibility flag.
         * \param v Boolean value for visibility.
         */
        visible(const bool& v);

        /*!
         * \brief Check if visible.
         * \return True if visible, false if not.
         */
        const bool check(void) const;

        /*!
         * \brief Show the entity.
         */
        void show(void);

        /*!
         * \brief Hide the entity.
         */
        void hide(void);

    private:
        bool is_visible;    //  Flag to set visibility.
};

} //  namespace cmp

} //  namespace wte

#endif
