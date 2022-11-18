/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/_debug/logger.hpp"

namespace wte {

std::ofstream logger::log_file;

/*
 *
 */
void logger::start(void) {
    std::time_t t = std::time(nullptr);
    std::ostringstream date_stream;
    date_stream << std::put_time(std::localtime(&t), "%d-%m-%Y_%H-%M-%S");
    std::string date = date_stream.str();
    std::cout << "Logging exceptions to:  exceptions_" + date + ".log.csv\n";
    log_file.open("exceptions_" + date + ".log.csv", std::ios::out | std::ios::trunc);
    log_file << "Description, Location, Time, Code" << std::endl;
}

/*
 *
 */
void logger::stop(void) {
    log_file.close();
}

/*
 *
 */
const void logger::log(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t)
{
    log_file <<
        d + ", " + l + ", " + std::to_string(c) + ", " + std::to_string(t) + "\n";
}

}  //  end namespace wte
