/*!
 * wtengine | File:  logger.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifdef WTE_DEBUG_MODE

namespace wte {

const bool& logger::is_running = logger::_is_running = false;

/*
 *
 */
const bool logger::add(const log_item& log_me) {
    try {
        _error_queue.push(log_me);
    } catch {
        return false;
    }
    return true;
}

/*
 *
 */
void logger::start(void) {
    if(_is_running == true) return;
    future_obj = exit_signal.get_future();
    std::thread th([&]() { run(); });
    th.detach();
    _is_running = true;
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
        //  todo:  process queue
    }
}

}  //  end namespace wte

#endif  //  WTE_DEBUG_MODE
