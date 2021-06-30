/*!
 * WTEngine | File:  wte_exception.hpp
 * 
 * \author Matthew Evans
 * \version 0.2
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
 * \brief .
 * 
 * .
 */
class wte_exception : public std::exception {
    public:
        inline wte_exception(const char* desc) : exception_desc(desc) {};
        inline ~wte_exception() {};

        /*!
         * \brief .
         * 
         * \param .
         * \return Thrown exception
         */
        virtual const char* what() const throw() {
            return exception_desc;
        }

    private:
        const char* exception_desc;
};

} //  end namespace wte

#endif
