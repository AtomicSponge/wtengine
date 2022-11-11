/*!
 * wtengine | File:  exceptions.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
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
engine_error::engine_error(const std::string& d) : item(exception_item(d.c_str(), "Engine", 1)) {
    if constexpr (build_options.debug_mode) logger::add(
        item.description, item.location, item.code, item.time);
}

/*
 *
 */
const char* engine_error::what() const noexcept { return item.description; }

/*
 *
 */
const char* engine_error::where() const noexcept { return item.location; }

/*
 *
 */
int64_t engine_error::when() const noexcept { return item.time; }

/*
 *
 */
engine_error::~engine_error() {
    std::cout << "ERROR:  ";
    std::cout << item.description << std::endl;
    std::exit(item.code);
}

/*
 *
 */
engine_exception::engine_exception(const std::string& d, const std::string& l, const uint& c) :
item(exception_item(d, l, c)) {
    if constexpr (build_options.debug_mode) logger::add(
        item.description, item.location, item.code, item.time);
}

/*
 *
 */
const char* engine_exception::what() const noexcept { return item.description; }

/*
 *
 */
const char* engine_exception::where() const noexcept { return item.location; };

/*
 *
 */
int64_t engine_exception::when() const noexcept { return item.time; };

/*
 *
 */
engine_exception::~engine_exception() {
    std::cout << "WARNING:  ";
    std::cout << item.description << std::endl;
    std::cout << "CODE:  ";
    std::cout << item.code;
    std::cout << "\tAT:  ";
    std::cout << item.time << std::endl;
}

}  //  end namespace wte
