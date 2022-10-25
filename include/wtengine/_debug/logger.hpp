/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_LOGGER_HPP)
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

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        static const void add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t);

        static const bool& is_running;  //!<  Flag to see if the logger is running.

    private:
        logger();
        ~logger();

        static const bool start(void);
        static void run(void);
        static void stop(void);

        static std::stack<
            std::tuple<std::string, std::string, uint, int64_t>
        > _error_queue;

        inline static bool _is_running = ([]{
            //return if(future_obj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
            return true;
        });

        static std::ofstream log_file;

        static std::promise<void> exit_signal;
        static std::future<void> future_obj;

        static std::mutex log_mtx;
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

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        static void add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t);

        static const bool is_running = false;

    private:
        logger() = default;
        ~logger() = default;
        
        static bool start(void);
        static void run(void);
        static void stop(void);

        static const bool _is_running = false;
};

template <typename... Args>
inline bool logger_add(Args... args)
{
    return false;
}

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
