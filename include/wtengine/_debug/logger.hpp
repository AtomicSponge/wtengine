/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_LOGGER_HPP)
#define WTE_LOGGER_HPP

#include <string>
#include <fstream>
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

    private:
        logger();
        ~logger();

        static void start(void);
        static void run(void);
        static void stop(void);

        static std::stack<
            std::tuple<std::string, std::string, uint, int64_t>
        > _error_queue;

        static std::ofstream log_file;

        static std::promise<void> exit_signal;
        static std::future<void> future_obj;

        static std::mutex log_mtx;
};

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
