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
        inline wte_exception(const char* desc) : description(desc), time(0) {};
        inline wte_exception(const char* desc, const int64_t& t) : description(desc), time(t) {};
        inline ~wte_exception() {};

        /*!
         * \brief Returns the description of the thrown exception.
         * \return Description of thrown exception.
         */
        const char* what() const noexcept override {
            return description;
        };

        const int64_t when() const noexcept {
            return time;
        }

    private:
        const char* description;  //  Exception description.
        const int64_t time;
};

} //  end namespace wte

#endif
