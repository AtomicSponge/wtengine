/*!
 * WTEngine | File:  wte_exception.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_EXCEPTION_HPP
#define WTE_EXCEPTION_HPP

#include <exception>

#if WTE_DEBUG_MODE
#include <fstream>
#endif

#include "wtengine/_globals/_defines.hpp"

namespace wte
{

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
            #if WTE_DEBUG_MODE
            log_exception(desc, loc, t);
            #endif
        };

        inline ~wte_exception() {};

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        inline const char* what() const noexcept override {
            return description;
        };

        /*!
         * \brief
         * \return
         */
        inline const char* where() const noexcept {
            return location;
        };

        /*!
         * \brief
         * \return
         */
        inline const int64_t when() const noexcept {
            return time;
        };

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const int64_t time;       //  Time of exception.

        #if WTE_DEBUG_MODE
        inline void log_exception(
            const char* desc,
            const char* loc,
            const int64_t& t
        ) {
            exception_log_file.open("wte_debug//exception_log.txt", std::ios::app);
            exception_log_file << "Time: " << t << "\t";
            exception_log_file << "Location: " << loc << "\t";
            exception_log_file << "Description: " << desc << std::endl;
            exception_log_file.close();
        };

        inline static std::ofstream exception_log_file;
        #endif
};

} //  end namespace wte

#endif
