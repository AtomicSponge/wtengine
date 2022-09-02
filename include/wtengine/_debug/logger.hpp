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

#include "wtengine/_globals/engine_time.hpp"

namespace wte {

class engine;
class logger;

/*!
 * \class
 */

class exception_item final {
    friend class logger;
    friend class runtime_error;
    friend class exception;

    public:
        /*!
         * \brief
         */
        inline exception_item(const std::string& d, const std::string& l, const uint& c) :
            description(d.c_str()), location(l.c_str()), code(c), time(engine_time::check()) {};

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const uint& code;         //  Code of error
        const int64_t time;       //  Time of exception.
};

#ifdef WTE_DEBUG_MODE

/*!
 * \class
 */
class logger final {
    friend class engine;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an item to the logger.
         * \param log_me Item to add.
         * \return True on success, else false.
         */
        static const bool add(const exception_item& log_me);

        static const bool& is_running;  //!<  Flag to see if the logger is running.

    private:
        logger();
        ~logger();

        static const bool start(void);
        static void run(void);
        static void stop(void);

        static std::stack<exception_item> _error_queue;
        static bool _is_running;

        static std::ofstream log_file;

        static std::promise<void> exit_signal;
        static std::future<void> future_obj;

        static std::mutex log_mtx;
};

#else  // not WTE_DEBUG_MODE

/*!
 * \class
 */
class logger final {
    friend class engine;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add an item to the logger.
         * Non-debug mode that just fails.
         * \param log_me Item to add.
         * \return False.
         */
        inline static const bool add(const exception_item& log_me) {
            return false;
        };

        inline static const bool start(void) { return false; };
        static void run(void) {};
        inline static void stop(void) {};

        inline static const bool is_running = false;

    private:
        inline logger() = default;
        inline ~logger() = default;

        inline static const bool _is_running = false;
};

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
