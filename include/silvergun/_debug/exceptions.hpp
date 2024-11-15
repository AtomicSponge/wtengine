/*
* silvergun
* --------
* By Matthew Evans
* See LICENSE.md for copyright information.
*/

#if !defined(SLV_EXCEPTION_HPP)
#define SLV_EXCEPTION_HPP

#include <string>
#include <fstream>
#include <exception>

#include "silvergun/_debug/logger.hpp"
#include "silvergun/_globals/_defines.hpp"
#include "silvergun/_globals/engine_time.hpp"

namespace slv {

/*
 * Creates an info object that can be passed to the engine exceptions.
 */
class exception_item final {
  friend class engine_error;
  friend class engine_exception;

  private:
    const char* description;  //  Exception description.
    const char* location;     //  Exception location.
    const unsigned int code;  //  Code of error
    const int64_t time;       //  Time of exception.

    exception_item() = delete;    //  Delete default constructor.
    ~exception_item() = default;  //  Default destructor.
    //  Create a new exception item.
    exception_item(const std::string& d, const std::string& l, const unsigned int& c) :
      description(d.c_str()), location(l.c_str()), code(c), time(engine_time::check()) {};
};

/*!
 * \class engine_error
 * \brief Throws an engine runtime error.
 *
 * Exceptions thrown this way will terminate the engine.
 */
class engine_error final : public std::exception {
  private:
    const exception_item item;  //  Store the exception item.

  public:
    /*!
     * \brief Create a new runtime error object.  Sets the location to Engine and code to 1.
     * \param d An exception description.
     */
    engine_error(const std::string& d) : item(exception_item(d, "Engine", 1)) {
      if constexpr (build_options.debug_mode) {
        logger::log(item.description, item.location, item.code, item.time);
      }
    };

    engine_error() = delete;  //  Delete default constructor.
    virtual ~engine_error() = default;  // Default destructor.

    /*!
     * \brief Returns the description of the thrown exception.
     * \return Description of thrown exception.
     */
    const char* what() const noexcept override {
      return item.description;
    };

    /*!
     * \brief Return the location the exception occured.
     * \return Location of thrown exception.
     */
    const char* where() const noexcept {
      return item.location;
    };

    /*!
     * \brief Return the time the exception occured.
     * \return Time of thrown exception.
     */
    int64_t when() const noexcept {
      return item.time;
    };
};

/*!
* \class engine_exception
* \brief Throws an internal engine exception.
*
* Exceptions thrown this way will not terminate the engine. \n
* If debugging is enabled, they will also be logged to file.
*/
class engine_exception final : public std::exception {
  private:
    const exception_item item;  //  Store the exception item.

  public:
    /*!
     * \brief Create an engine exception.
     * \param d Description of exception.
     * \param l Location in engine of exception.
     * \param c Code of exception.
     */
    engine_exception(const std::string& d, const std::string& l, const unsigned int& c) :
      item(exception_item(d, l, c)) {
      if constexpr (build_options.debug_mode) {
        logger::log(item.description, item.location, item.code, item.time);
      }
    };

    engine_exception() = delete;  //  Delete default constructor.
    virtual ~engine_exception() = default;  // Default destructor.

    /*!
     * \brief Returns the description of the thrown exception.
     * \return Description of thrown exception.
     */
    const char* what() const noexcept override {
      return item.description;
    };

    /*!
     * \brief Return the location the exception occured.
     * \return Location of thrown exception.
     */
    const char* where() const noexcept {
      return item.location;
    };

    /*!
     * \brief Return the time the exception occured.
     * \return Time of thrown exception.
     */
    int64_t when() const noexcept {
      return item.time;
    };
};

}

#endif
