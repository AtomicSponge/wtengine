/*!
 * WTEngine | File:  engine_time.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/engine_time.hpp"

namespace wte
{

int64_t engine_time::current_time = 0;

engine_time::engine_time() {};
engine_time::~engine_time() {};

/*
 *
 */
const std::int64_t engine_time::check_time(void) { return current_time; };

/*
 *
 */
void engine_time::set_time(const std::int64_t& t) {  current_time = t; };

}  //  end namespace wte
