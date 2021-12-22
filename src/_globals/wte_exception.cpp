/*!
 * WTEngine | File:  wte_exception.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/_globals/wte_exception.hpp"

namespace wte {

/*
 *
 */
const char* wte_exception::what() const noexcept { return description; }

/*
 *
 */
const char* wte_exception::where() const noexcept { return location; }

/*
 *
 */
const int64_t wte_exception::when() const noexcept { return time; }

/*
 *
 */
void wte_exception::log_exception(
    const char* desc,
    const char* loc,
    const int64_t& t
) {
    std::ofstream exception_log_file("wte_debug//exception_log.txt", std::ios::app);
    exception_log_file << "Time: " << t << "\t";
    exception_log_file << "Location: " << loc << "\t";
    exception_log_file << "Description: " << desc << std::endl;
    exception_log_file.close();
}

}  //  end namespace wte
