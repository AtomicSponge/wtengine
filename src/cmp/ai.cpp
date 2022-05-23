/*!
 * WTEngine | File:  ai.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
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

}  //  end namespace wte::cmp
