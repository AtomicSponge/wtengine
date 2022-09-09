/*!
 * wtengine | File:  exceptions.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/_debug/exceptions.hpp"

namespace wte {

/*
 *
 */
exception_item::exception_item(const std::string& d, const std::string& l, const uint& c) :
    description(d.c_str()), location(l.c_str()), code(c), time(engine_time::check()) {};

/*
 *
 */
runtime_error::runtime_error(const exception_item& i) : item(i) {
    if constexpr (build_options.debug_mode) logger_add(
        i.description, i.location, i.code, i.time);
}

/*
 *
 */
const char* runtime_error::what() const noexcept { return item.description; }

/*
 *
 */
const char* runtime_error::where() const noexcept { return item.location; }

/*
 *
 */
const int64_t runtime_error::when() const noexcept { return item.time; }

/*
 *
 */
runtime_error::~runtime_error() { std::exit(item.code); }

/*
 *
 */
exception::exception(const exception_item& i) : item(i) {
    if constexpr (build_options.debug_mode) logger_add(
        i.description, i.location, i.code, i.time);
}

/*
 *
 */
const char* exception::what() const noexcept { return item.description; }

/*
 *
 */
const char* exception::where() const noexcept { return item.location; };

/*
 *
 */
const int64_t exception::when() const noexcept { return item.time; };

}  //  end namespace wte
