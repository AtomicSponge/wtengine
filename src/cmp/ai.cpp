/*!
 * WTEngine | File:  ai.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/ai.hpp"

namespace wte
{

namespace cmp
{

ai::ai(
    void func(const entity_id&)
) : enabled_ai(func), disabled_ai([](const entity_id& e_id){}) {}

ai::ai(
    void func_a(const entity_id&),
    void func_b(const entity_id&)
) : enabled_ai(func_a), disabled_ai(func_b) {}

void ai::run_enabled(const entity_id& e_id) {
    enabled_ai(e_id);
}

void ai::run_disabled(const entity_id& e_id) {
    disabled_ai(e_id);
}

} //  namespace cmp

} //  namespace wte
