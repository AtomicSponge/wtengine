/*!
 * \brief WTEngine | File:  render_order.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Render order component.
 */

#ifndef WTE_CMP_RENDER_ORDER_HPP
#define WTE_CMP_RENDER_ORDER_HPP

#include "../component.hpp"

namespace wte
{

namespace cmp
{

//! Render Order component
/*!
 * Used to sort entities in the Renderer
 */
class render_order : public component {
    public:
        /*!
         * Render order
         * \param void
         * \return void
         */
        inline render_order(int o) : order(o) {};

        int order;
};

} //  namespace cmp

} //  namespace wte

#endif
