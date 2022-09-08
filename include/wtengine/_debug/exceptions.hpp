/*!
 * wtengine | File:  wte_exceptions.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_EXCEPTION_HPP
#define WTE_EXCEPTION_HPP

#include <string>
#include <fstream>
#include <exception>

#include "wtengine/_debug/logger.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"

namespace wte {

/*!
 * \class
 */
class exception_item final {
    friend class logger;
    friend class runtime_error;
    friend class exception;

    public:
        /*!
         * \brief
         */
        inline exception_item(const std::string& d, const std::string& l, const uint& c) :
            description(d.c_str()), location(l.c_str()), code(c), time(engine_time::check()) {};

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const uint code;          //  Code of error
        const int64_t time;       //  Time of exception.
};

/*
 * Log an exception to file
 */
inline void log_exception(
    const std::string& d,
    const std::string& l,
    const uint& c,
    const int64_t& t)
{
    logger::add(d, l, c, t);
};

/*!
 * \class runtime_error
 * \brief Throws an engine runtime error.
 *
 * Exceptions thrown this way will terminate the engine.
 */
class runtime_error final : public std::exception {
    friend void log_exception(const exception_item& item);

    public:
        /*!
         * \brief Create a new runtime error object.
         * \param i An exception item object.
         */
        inline runtime_error(const exception_item& i) : item(i) {
            if constexpr (build_options.debug_mode) log_exception(i.description, i.location, i.code, i.time);
        };

        runtime_error() = delete;    //!<  Delete default constructor.

        /*!
         * \brief Terminate engine after handling exception.
         */
        inline virtual ~runtime_error() {
            std::exit(item.code);
        };

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        inline const char* what() const noexcept override { return item.description; };

        /*!
         * \brief Return the location the exception occured.
         * \return Location of thrown exception.
         */
        inline const char* where() const noexcept{ return item.location; };

        /*!
         * \brief Return the time the exception occured.
         * \return Time of thrown exception.
         */
        inline const int64_t when() const noexcept { return item.time; };

    private:
        const exception_item item;  //  Store the exception item.
};

/*!
 * \class exception
 * \brief Throws an internal engine exception.
 *
 * Exceptions thrown this way will not terminate the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class exception final : public std::exception {
    friend void log_exception(const exception_item& item);

    public:
        /*!
         * \brief Create a wte exception.
         * \param i An exception item object.
         */
        inline exception(const exception_item& i) : item(i) {
            if constexpr (build_options.debug_mode) log_exception(i.description, i.location, i.code, i.time);
        };

        exception() = delete;    //!<  Delete default constructor.
        ~exception() = default;  //!<  Default destructor.

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        inline const char* what() const noexcept override { return item.description; };

        /*!
         * \brief Return the location the exception occured.
         * \return Location of thrown exception.
         */
        inline const char* where() const noexcept{ return item.location; };

        /*!
         * \brief Return the time the exception occured.
         * \return Time of thrown exception.
         */
        inline const int64_t when() const noexcept { return item.time; };

    private:
        const exception_item item;  //  Store the exception item.
};

}  //  end namespace wte

#endif
