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

/*
 *
 */
void logger::add(const log_item& log_me) {
    _error_queue.push(log_me);
}

/*
 *
 */
void logger::run(void) {
    while(_is_running) {
        //
    }
}

}  //  end namespace wte

#endif  //  WTE_DEBUG_MODE
