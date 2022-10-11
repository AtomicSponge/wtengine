/*!
 * wtengine | File:  logger.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.3
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
    std::time_t t = std::time(nullptr);
    std::ostringstream date_stream;
    date_stream << std::put_time(std::localtime(&t), "%d-%m-%Y_%H-%M-%S");
    std::string date = date_stream.str();
    std::cout << "Logging exceptions to:  exceptions_" + date + ".log.csv";
    log_file.open("exceptions_" + date + ".log.csv", std::ios::out | std::ios::trunc);
    log_file << "New log:  " + date + "\n";
    log_file << "Description, Location, Time, Code\n\n";
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
    future_obj = exit_signal.get_future();
    std::thread th([&]() { run(); });
    th.detach();
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
const void logger::add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t)
{
    log_mtx.lock();
    _error_queue.push(std::make_tuple(d, l, c, t));
    log_mtx.unlock();
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
const void logger::add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t) {}

#endif  //  WTE_DEBUG_MODE

}  //  end namespace wte
