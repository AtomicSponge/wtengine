/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_LOGGER_HPP
#define WTE_LOGGER_HPP

#ifdef WTE_DEBUG_MODE

#include <fstream>
#include <string>
#include <stack>
#include <exception>

#include "wtengine/_globals/wte_exceptions.hpp"

namespace wte {

class logger final {
    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an item to the logger.
         * \param log_me Item to add.
         * \return True on success, else false.
         */
        const bool add(const log_item& log_me);

    private:
        logger() = default;
        ~logger() = default;

        void run(void) {};

        static std::stack<log_item> _error_queue;
        static bool _is_running;
}

}  //  end namespace wte

#else  // not WTE_DEBUG_MODE

class logger final {
    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an item to the logger.
         * Non-debug mode that just fails.
         * \param log_me Item to add.
         * \return False.
         */
        inline const bool add(const log_item& log_me) {
            return false;
        };

    private:
        inline logger() = default;
        inline ~logger() = default;

        inline static bool _is_running = false;
}

#endif  //  WTE_DEBUG_MODE

#endif  //  WTE_LOGGER_HPP
