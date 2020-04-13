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

//! Visible component
/*!
  Toggles if an entity will be drawn by the Renderer
*/
class visible final : public component {
    public:
        inline visible() : is_visible(true) {};
        inline visible(bool v) : is_visible(v) {};

        bool is_visible;
};

} //  namespace cmp

} //  namespace wte

#endif
