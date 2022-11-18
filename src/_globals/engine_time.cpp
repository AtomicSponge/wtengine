/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/_globals/engine_time.hpp"

namespace wte {

int64_t engine_time::current_time = 0;

/*
 *
 */
std::int64_t engine_time::check(void) { return current_time; };

/*
 *
 */
void engine_time::set(const std::int64_t& t) {  current_time = t; };

}  //  end namespace wte
