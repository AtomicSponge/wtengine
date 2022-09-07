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

#if WTE_DEBUG_MODE  //  Debug mode set if true

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
        inline static const bool add(const exception_item& log_me) {
            try {
                log_mtx.lock();
                _error_queue.push(log_me);
                log_mtx.unlock();
            } catch {
                return false;
            }
            return true;
        };

        inline static const bool& is_running = _is_running;  //!<  Flag to see if the logger is running.

    private:
        inline logger() {
            // create new log file
            try {
                log_file.open("wte-logs\exception_log_" +
                    std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") + ".log", ios::out | ios::trunc);
                log_file << "New log\n\n";
            } catch {
                throw runtime_error("Error creating log file!");
            }
        };

        inline ~logger() {
            stop();
            log_file.close();
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
                    log_item temp_log_item = mystack.top();

                    //  Mutex pop the stack
                    log_mtx.lock();
                    mystack.pop();
                    log_mtx.unlock();

                    //  Process item
                    log_file <<
                        "Description:  " + std::to_string(temp_log_item.description) + "\n" +
                        "Location:  " + std::to_string(temp_log_item.location) + "\n" +
                        "Time:  " + std::to_string(temp_log_item.time) + "\n";
                        "Code:  " + std::to_string(temp_log_item.code) + "\n\n";
                }

            }
        };

        inline static void stop(void) {
            exit_signal.set_value();
            _is_running = false;
            exit_signal = std::promise<void>();
        };

        inline static std::stack<exception_item> _error_queue;
        inline static bool _is_running;

        inline static std::ofstream log_file;

        inline static std::promise<void> exit_signal;
        inline static std::future<void> future_obj;

        inline static std::mutex log_mtx;
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

        inline static const bool is_running = false;

    private:
        inline logger() = default;
        inline ~logger() = default;

        inline static const bool start(void) { return false; };
        static void run(void) {};
        inline static void stop(void) {};

        inline static const bool _is_running = false;
};

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
