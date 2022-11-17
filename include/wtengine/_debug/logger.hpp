/*!
 * wtengine | File:  logger.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.1
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

namespace wte {

class engine;

/*!
 * \class logger
 * \brief Logs exceptions to file.  This option is built when the engine is in debug mode.
 */
class logger final {
    friend class engine;

    public:
        logger(const logger&) = delete;          //!<  Delete copy constructor.
        void operator=(logger const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Add exception information to the logger.
         * \param d Description of item.
         * \param l Location of item.
         * \param c Error code of item.
         * \param t Engine time of item.
         */
        static const void log(
            const std::string& d, const std::string& l,
            const uint& c, const int64_t& t);

    private:
        logger() = default;
        ~logger() = default;

        static void start(void);
        static void stop(void);

        static std::ofstream log_file;
};

}  //  end namespace wte

#endif  //  WTE_LOGGER_HPP
