/*!
 * \brief WTEngine | File:  name.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Name component.
 */

#ifndef WTE_CMP_NAME_HPP
#define WTE_CMP_NAME_HPP

#include <string>

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Name component
/*!
 * Store the name of an entity
 */
class name final : public component {
    public:
        inline name(std::string n) : name_str(n) {};

        std::string name_str;
};

} //  namespace cmp

} //  namespace wte

#endif
