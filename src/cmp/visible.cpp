/*!
 * WTEngine | File:  visible.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/visible.hpp"

namespace wte
{

namespace cmp
{

/*
 *
 */
visible::visible() : is_visible(true) {}

/*
 *
 */
visible::visible(const bool v) : is_visible(v) {}

/*
 *
 */
const bool visible::check(void) const { return is_visible; }

/*
 *
 */
void visible::show(void) { is_visible = true; }

/*
 *
 */
void visible::hide(void) { is_visible = false; }

} //  namespace cmp

} //  namespace wte
