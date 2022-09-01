/*!
 * wtengine | File:  logger.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifdef WTE_DEBUG_MODE

namespace wte {

const bool& logger::is_running = logger::_is_running = false;

/*
 *
 */
logger::logger() {
    // create new log file
    try {
        log_file("fjkdlsjfkdj.log");
        log_file << "New log\n\n";
    } catch {
        throw runtime_error("Error creating log file!");
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
const bool logger::add(const log_item& log_me) {
    try {
        log_lock.lock();
        _error_queue.push(log_me);
        log_lock.unlock();
    } catch {
        return false;
    }
    return true;
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
        return false;
    }
    return _is_running = true;
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
void logger::run(void) {
    while(future_obj.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout) {
        //  Run this as a loop until the thread stops.
        if(!_error_queue.empty()) {
            //  Get next item
            log_item temp_log_item = mystack.top();

            //  Mutex pop the stack
            log_lock.lock();
            mystack.pop();
            log_lock.unlock();

            //  Process item
            log_file <<
                "Description:  " + std::to_string(temp_log_item.description) + "\n" +
                "Location:  " + std::to_string(temp_log_item.location) + "\n" +
                "Time:  " + std::to_string(temp_log_item.time) + "\n\n";
        }

    }
}

}  //  end namespace wte

#endif  //  WTE_DEBUG_MODE
