/*!
 * wtengine | File:  logger.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/_debug/logger.hpp"

namespace wte {

#if WTE_DEBUG_MODE  //  Debug mode set if true

/*
 *
 */
logger::logger() {
    // create new log file
    try {
        std::time_t t std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        log_file.open("exception_log_" +
            std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") + ".log.csv", std::ios::out | std::ios::trunc);
        log_file << "New log:  " + std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") + "\n";
        log_file << "Description, Location, Time, Code\n\n"

    } catch {
        throw wte::runtime_error("Error creating log file!");
    }
}

/*
 *
 */
logger::~logger() {
    stop();
    log_file.close();
}

/*
 *
 */
const bool logger::start(void) {
    if(_is_running == true) return false;
    try {
        future_obj = exit_signal.get_future();
        std::thread th([&]() { run(); });
        th.detach();
    } catch {
        throw wte::runtime_error("Unable to start logger!");
    }
    return _is_running = true;
}

/*
 *
 */
void logger::run(void) {
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
                std::get<0>(log_item) + ", " +
                std::get<1>(log_item) + ", " +
                std::to_string(std::get<2>(log_item)) + ", ";
                std::to_string(std::get<3>(log_item)) + "\n";
        }
    }
}

/*
 *
 */
void logger::stop(void) {
    exit_signal.set_value();
    _is_running = false;
    exit_signal = std::promise<void>();
}

/*
 *
 */
const bool logger::add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t)
{
    try {
        log_mtx.lock();
        _error_queue.push(std::make_tuple(d, l, c, t));
        log_mtx.unlock();
    } catch {
        throw engine_exception("Issue adding exception to log queue!", "Logger", 2);
        return false;
    }
    return true;
}

#else  // not WTE_DEBUG_MODE

/*
 *
 */
const bool logger::start(void) { return false; }

/*
 *
 */
void logger::run(void) {}

/*
 *
 */
void logger::stop(void) {}

/*
 *
 */
const bool logger::add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t) { return false; }

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte
