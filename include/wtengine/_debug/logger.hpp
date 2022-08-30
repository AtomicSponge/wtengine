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

class logger final {
    public:
        void add(const log_item& log_me);

    private:
        void run(void) {};

        std::stack<log_item> _error_queue;
}

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP

#endif  //  WTE_DEBUG_MODE
