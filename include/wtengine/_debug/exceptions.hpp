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

#include <fstream>
#include <exception>

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"

namespace wte {

class logger;

class exception_item final {
    friend class logger;

    public:
        inline exception_item(const char* d, const char* l) :
            description(d), location(l), time(engine_time::check()) {};

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const int64_t time;       //  Time of exception.
};

/*!
 * \class runtime_error
 * \brief Throws an internal engine exception that terminates the application.
 */
class runtime_error final : public std::exception {
    public:
        inline runtime_error(const char* desc) : description(desc) {
            if constexpr (build_options.debug_mode) log_exception(desc);
        };

        runtime_error() = delete;    //!<  Delete default constructor.
        ~runtime_error() = default;  //!<  Default destructor.

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        const char* what() const noexcept override;

    private:
        const char* description;  //  Exception description.
        //  Log exception to file when debugging is enabled.
        inline void log_exception(const char* desc) {};
};

/*!
 * \class exception
 * \brief Throws an internal engine exception.
 *
 * Exceptions thrown this way will be displayed as an alert in the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class exception final : public std::exception {
    public:
        /*!
         * \brief Create a wte exception.
         * \param desc Description of thrown exception.
         * \param loc Location exception was thrown.
         * \param t Time exception was thrown.
         */
        inline exception(
            const char* desc,
            const char* loc,
            const int64_t& t
        ) : description(desc), location(loc), time(t) {
            if constexpr (build_options.debug_mode) log_exception(desc, loc, t);
        };

        exception() = delete;    //!<  Delete default constructor.
        ~exception() = default;  //!<  Default destructor.

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        const char* what() const noexcept override;

        /*!
         * \brief Return the location the exception occured.
         * \return Location of thrown exception.
         */
        const char* where() const noexcept;

        /*!
         * \brief Return the time the exception occured.
         * \return Time of thrown exception.
         */
        const int64_t when() const noexcept;

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const int64_t time;       //  Time of exception.
        //  Log exception to file when debugging is enabled.
        void log_exception(
            const char* desc,
            const char* loc,
            const int64_t& t
        );
};

}  //  end namespace wte

#endif
