/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_LOGGER_HPP
#define WTE_LOGGER_HPP

#include <fstream>
#include <string>
#include <stack>
#include <exception>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#include "wtengine/_globals/wte_exceptions.hpp"

namespace wte {

#ifdef WTE_DEBUG_MODE

/*!
 * \class
 */
class logger final {
    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an item to the logger.
         * \param log_me Item to add.
         * \return True on success, else false.
         */
        static const bool add(const exception_item& log_me);

        /*!
         * \brief Start the logger.
         */
        static void start(void);

        /*!
         * \brief Stop the logger.
         */
        static void stop(void);

        static const bool& is_running;  //!<  Flag to see if the logger is running.

    private:
        logger() = default;
        ~logger();

        static void run(void) {};

        static std::stack<exception_item> _error_queue;
        static bool _is_running;

        std::promise<void> exit_signal;
        std::future<void> future_obj;
        std::mutex log_lock;
};

#else  // not WTE_DEBUG_MODE

/*!
 * \class
 */
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
        inline static const bool add(const log_item& log_me) {
            return false;
        };

        inline static const bool& is_running = _is_running;

    private:
        inline logger() = default;
        inline ~logger() = default;

        inline static const bool _is_running = false;
};

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
