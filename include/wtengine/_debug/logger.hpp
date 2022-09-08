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

#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <stack>
#include <tuple>
#include <exception>
#include <thread>
#include <future>
#include <mutex>
#include <chrono>

#include "wtengine/_globals/engine_time.hpp"

namespace wte {

class engine;
class logger;

void log_exception(const std::string& d, const std::string& l, const uint& c, const int64_t& t);

#if WTE_DEBUG_MODE  //  Debug mode set if true

/*!
 * \class logger
 * \brief Logs exceptions to file.  This option is built when the engine is in debug mode.
 */
class logger final {
    friend class engine;
    friend void log_exception(const std::string& d, const std::string& l, const uint& c, const int64_t& t);

    private:
        inline logger() {
            // create new log file
            try {
                log_file.open("wte-logs\exception_log_" +
                    std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") + ".log", std::ios::out | std::ios::trunc);
                log_file << "New log\n\n";
            } catch {
                throw runtime_error("Error creating log file!");
            }
        };

        inline ~logger() {
            stop();
            log_file.close();
        };

        inline static const bool add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t)
        {
            try {
                log_mtx.lock();
                _error_queue.push(std::make_tuple(d, l, c, t));
                log_mtx.unlock();
            } catch {
                return false;
            }
            return true;
        };

        inline static const bool start(void) {
            if(_is_running == true) return false;
            try {
                future_obj = exit_signal.get_future();
                std::thread th([&]() { run(); });
                th.detach();
            } catch {
                return false;
            }
            return _is_running = true;
        };

        inline static void run(void) {
            while(future_obj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
                //  Run this as a loop until the thread stops.
                if(!_error_queue.empty()) {
                    //  Get next item
                    auto log_item = _error_queue.top();

                    //  Mutex pop the stack
                    log_mtx.lock();
                    _error_queue.pop();
                    log_mtx.unlock();

                    //  Process item
                    log_file <<
                        "Description:  " + std::get<0>(log_item) + "\n" +
                        "Location:  " + std::get<1>(log_item) + "\n" +
                        "Time:  " + std::to_string(std::get<2>(log_item)) + "\n";
                        "Code:  " + std::to_string(std::get<3>(log_item)) + "\n\n";
                }

            }
        };

        inline static void stop(void) {
            exit_signal.set_value();
            _is_running = false;
            exit_signal = std::promise<void>();
        };

        inline static std::stack<
            std::tuple<std::string, std::string, uint, int64_t>
        > _error_queue;
        inline static bool _is_running;

        inline static std::ofstream log_file;

        inline static std::promise<void> exit_signal;
        inline static std::future<void> future_obj;

        inline static std::mutex log_mtx;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        inline static const bool& is_running = _is_running;  //!<  Flag to see if the logger is running.
};

#else  // not WTE_DEBUG_MODE

/*!
 * \class logger
 * \brief Skip logging.  This option is built when the engine is NOT in debug mode.
 */
class logger final {
    friend class engine;
    friend void log_exception(const std::string& d, const std::string& l, const uint& c, const int64_t& t);

    private:
        inline logger() = default;
        inline ~logger() = default;

        inline static const bool add(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t) { return false; };
        
        inline static const bool start(void) { return false; };
        static void run(void) {};
        inline static void stop(void) {};

        inline static const bool _is_running = false;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        inline static const bool is_running = false;
};

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
