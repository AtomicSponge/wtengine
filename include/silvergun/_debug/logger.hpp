/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_LOGGER_HPP)
#define SLV_LOGGER_HPP

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

namespace slv {

class engine;

/*!
 * \class logger
 * \brief Logs exceptions to file.  This option is built when the engine is in debug mode.
 */
class logger final {
  friend class engine;

  private:
    logger() = default;
    ~logger() = default;

    static void start(void) {
      std::time_t t = std::time(nullptr);
      std::ostringstream date_stream;
      date_stream << std::put_time(std::localtime(&t), "%d-%m-%Y_%H-%M-%S");
      std::string date = date_stream.str();
      std::cout << "Logging exceptions to:  exceptions_" + date + ".log.csv\n";
      log_file.open("exceptions_" + date + ".log.csv", std::ios::out | std::ios::trunc);
      log_file << "Description, Location, Time, Code" << std::endl;
    };

    static void stop(void) {
      log_file.close();
    };

    inline static std::ofstream log_file;

  public:
    logger(const logger&) = delete;          //  Delete copy constructor.
    void operator=(logger const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Add exception information to the logger.
     * \param d Description of item.
     * \param l Location of item.
     * \param c Error code of item.
     * \param t Engine time of item.
     */
    static const void log(
      const std::string& d, const std::string& l,
      const unsigned int& c, const int64_t& t) {
      log_file << d + ", " + l + ", " + std::to_string(c) + ", " + std::to_string(t) + "\n";
    };
};

}

#endif
