/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifdef WTE_DEBUG_MODE

#ifndef WTE_LOGGER_HPP
#define WTE_LOGGER_HPP

#include <fstream>
#include <string>
#include <stack>
#include <exception>

#include "wtengine/_globals/wte_exceptions.hpp"

namespace wte {

struct log_item final {
    const char* description;  //  Exception description.
    const char* location;     //  Exception location.
    const int64_t time;       //  Time of exception.
};

class logger final {
    public:
        const void add(const std::string& message)

    private:
        std::stack<log_item> _error_queue;
}

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP

#endif  //  WTE_DEBUG_MODE
