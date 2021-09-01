/*!
 * WTEngine | File:  ai.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/cmp/ai.hpp"

namespace wte::cmp {

ai::ai(
    const std::function<void(const entity_id&)>& func
) : enabled(true), enabled_ai(func), disabled_ai([](const entity_id& e_id){}) {}

ai::ai(
    const std::function<void(const entity_id&)>& func_a,
    const std::function<void(const entity_id&)>& func_b
) : enabled(true), enabled_ai(func_a), disabled_ai(func_b) {}

void ai::run_enabled(const entity_id& e_id) {
    try { enabled_ai(e_id); } catch(...) { throw; }
}

void ai::run_disabled(const entity_id& e_id) {
    try { disabled_ai(e_id); } catch(...) { throw; }
}

/*
 *
 */
const bool ai::status(void) const { return enabled; }

/*
 *
 */
void ai::enable(void) { enabled = true; }

/*
 *
 */
void ai::disable(void) { enabled = false; }

}  //  end namespace wte::cmp
