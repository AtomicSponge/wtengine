/*!
 * \brief WTEngine | File:  enabled.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Enabled component.
 */

#ifndef WTE_CMP_ENABLED_HPP
#define WTE_CMP_ENABLED_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Enabled component
/*!
 * Toggle an entity to enabled or disabled state
 */
class enabled final : public component {
    public:
        inline enabled() : is_enabled(true) {};
        inline enabled(bool e) : is_enabled(e) {};

        bool is_enabled;
};

} //  namespace cmp

} //  namespace wte

#endif
