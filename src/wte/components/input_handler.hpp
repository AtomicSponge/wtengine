/*!
 * \brief WTEngine | File:  input_handler.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input handler component.  Tag to be processed by input system.
 */

#ifndef WTE_CMP_INPUT_HANDLER_HPP
#define WTE_CMP_INPUT_HANDLER_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Input Handler component
/*!
  Tag an entity to be processed by the Input system
*/
class input_handler final : public component {
    //  No members
};

} //  namespace cmp

} //  namespace wte

#endif
