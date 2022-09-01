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

/*
 * Log an exception to file
 */
inline void log_exception(const exception_item& item) {
    //logger::add(item);
};

/*!
 * \class runtime_error
 * \brief Throws an internal engine exception that terminates the application.
 */
class runtime_error final : public std::exception {
    friend void log_exception(const exception_item& item);

    public:
        /*!
         * \brief
         */
        inline runtime_error(const exception_item& i) : item(i) {
            if constexpr (build_options.debug_mode) log_exception(item);
        };

        /*!
         * \brief
         */
        runtime_error() = delete;    //!<  Delete default constructor.
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
 * Exceptions thrown this way will be displayed as an alert in the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class exception final : public std::exception {
    friend void log_exception(const exception_item& item);

    public:
        /*!
         * \brief Create a wte exception.
         * \param desc Description of thrown exception.
         * \param loc Location exception was thrown.
         * \param t Time exception was thrown.
         */
        inline exception(const exception_item& i) : item(i) {
            //if constexpr (build_options.debug_mode) log_exception(i);
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
        //  Log exception to file when debugging is enabled.
};

}  //  end namespace wte

#endif
