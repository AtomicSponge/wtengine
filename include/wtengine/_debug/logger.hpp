/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_LOGGER_HPP
#define WTE_LOGGER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <stack>
#include <tuple>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#include "wtengine/_globals/engine_time.hpp"

namespace wte {

class engine;
class logger;

#if WTE_DEBUG_MODE  //  Debug mode set if true

/*!
 * \class logger
 * \brief Logs exceptions to file.  This option is built when the engine is in debug mode.
 */
class logger final {
    friend class engine;

    private:
        logger();
        ~logger();

        static const bool start(void);
        static void run(void);
        static void stop(void);

        static std::stack<
            std::tuple<std::string, std::string, uint, int64_t>
        > _error_queue;
        static bool _is_running;

        static std::ofstream log_file;

        static std::promise<void> exit_signal;
        static std::future<void> future_obj;

        static std::mutex log_mtx;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        static const bool add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t);

        static const bool& is_running;  //!<  Flag to see if the logger is running.
};

inline const bool logger_add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t)
{
    try {
        logger::add(d, l, c, t);
    } catch (const std::exception& e) { 
        return false;
    }
    return true;
}

#else  // not WTE_DEBUG_MODE

/*!
 * \class logger
 * \brief Skip logging.  This option is built when the engine is NOT in debug mode.
 */
class logger final {
    friend class engine;

    private:
        logger() = default;
        ~logger() = default;
        
        static const bool start(void);
        static void run(void);
        static void stop(void);

        static const bool _is_running = false;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        static const bool add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t);

        static const bool is_running = false;
};

inline const bool logger_add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t)
{
    return false;
}

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
