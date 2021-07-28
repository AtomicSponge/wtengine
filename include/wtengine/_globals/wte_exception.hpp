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
 * Exceptions thrown this way will be displayed as an alert in the engine.
 */
class wte_exception final : public std::exception {
    public:
        /*!
         * \brief Create a wte_exception.
         * \param desc Exception description.
         */
        inline wte_exception(
            const char* desc
        ) : description(desc), time(-1) {
            #if WTE_DEBUG_MODE
            log_exception(desc, -1);
            #endif
        };

        inline wte_exception(
            const char* desc,
            const int64_t& t
        ) : description(desc), time(t) {
            #if WTE_DEBUG_MODE
            log_exception(desc, t);
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

        inline const int64_t when() const noexcept {
            return time;
        };

    private:
        const char* description;  //  Exception description.
        const int64_t time;

        #if WTE_DEBUG_MODE
        inline void log_exception(
            const char* desc,
            const int64_t& t
        ) {
            exception_log_file.open("wte_debug//exception_log.txt", std::ios::app);
            exception_log_file << t << "\t" << desc << std::endl;
            exception_log_file.close();
        };

        inline static std::ofstream exception_log_file;
        #endif
};

} //  end namespace wte

#endif
