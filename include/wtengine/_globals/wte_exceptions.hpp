/*!
 * wtengine | File:  wte_exception.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_EXCEPTION_HPP
#define WTE_EXCEPTION_HPP

#include <fstream>
#include <exception>

#include "wtengine/_globals/_defines.hpp"

namespace wte {

class log_item final {
    public:
        inline log_item(const char& d, const char& l) :
            description(d), location(l) {
                time = 42;
        };

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const int64_t time;       //  Time of exception.
};

/*!
 * \class wte_runtime_error
 * \brief Throws an internal engine exception.
 *
 * Throws both std::runtime_error and logs if enabled.
 */
class wte_runtime_error final : public std::runtime_error {
};

/*!
 * \class wte_exception
 * \brief Throws an internal engine exception.
 *
 * Exceptions thrown this way will be displayed as an alert in the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class wte_exception final : public std::exception {
    public:
        /*!
         * \brief Create a wte_exception.
         * \param desc Description of thrown exception.
         * \param loc Location exception was thrown.
         * \param t Time exception was thrown.
         */
        inline wte_exception(
            const char* desc,
            const char* loc,
            const int64_t& t
        ) : description(desc), location(loc), time(t) {
            if constexpr (build_options.debug_mode) log_exception(desc, loc, t);
        };

        wte_exception() = delete;    //!<  Delete default constructor.
        ~wte_exception() = default;  //!<  Default destructor.

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
