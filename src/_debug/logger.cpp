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

logger::logger() {
    // create new log file
    try {
        log_file.open("wte-logs\exception_log_" +
            std::put_time(&tm, "%d-%m-%Y_%H-%M-%S") + ".log", std::ios::out | std::ios::trunc);
        log_file << "New log\n\n";
    } catch {
        throw wte::runtime_error("Error creating log file!");
    }
}

logger::~logger() {
    stop();
    log_file.close();
}

const bool logger::start(void) {
    if(_is_running == true) return false;
    try {
        future_obj = exit_signal.get_future();
        std::thread th([&]() { run(); });
        th.detach();
    } catch {
        return false;
    }
    return _is_running = true;
}

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
                "Description:  " + std::get<0>(log_item) + "\n" +
                "Location:  " + std::get<1>(log_item) + "\n" +
                "Time:  " + std::to_string(std::get<2>(log_item)) + "\n";
                "Code:  " + std::to_string(std::get<3>(log_item)) + "\n\n";
        }
    }
}

void logger::stop(void) {
    exit_signal.set_value();
    _is_running = false;
    exit_signal = std::promise<void>();
}

const bool add(
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
}

#else  // not WTE_DEBUG_MODE

const bool start(void) { return false; }
void run(void) {}
void stop(void) {}

static const bool add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t) { return false; }

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte