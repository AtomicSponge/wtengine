/*!
 * wtengine | File:  exceptions.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_EXCEPTION_HPP)
#define WTE_EXCEPTION_HPP

#include <string>
#include <fstream>
#include <exception>

#include "wtengine/_debug/logger.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"

namespace wte {

/*!
 * \class exception_item
 * \brief Creates an info object that can be passed to the engine exceptions.
 */
class exception_item final {
    friend class engine_error;
    friend class engine_exception;

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const uint code;          //  Code of error
        const int64_t time;       //  Time of exception.
    
        exception_item() = delete;    //  Delete default constructor.
        ~exception_item() = default;  //  Default destructor.
        //Create a new exception item.
        exception_item(const std::string& d, const std::string& l, const uint& c);
};

/*!
 * \class runtime_error
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
        engine_error(const std::string& d);

        engine_error() = delete;    //!<  Delete default constructor.

        /*!
         * \brief Terminates engine after handling exception.
         */
        virtual ~engine_error();

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
        int64_t when() const noexcept;
};

/*!
 * \class exception
 * \brief Throws an internal engine exception.
 *
 * Exceptions thrown this way will not terminate the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class engine_exception final : public std::exception {
    public:
        /*!
         * \brief Create an engine exception.
         * \param d Description of exception.
         * \param l Location in engine of exception.
         * \param c Code of exception.
         */
        engine_exception(const std::string& d, const std::string& l, const uint& c);

        engine_exception() = delete;  //!<  Delete default constructor.

        /*!
         * \brief Prints exception to console.
         */
        virtual ~engine_exception();

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
        int64_t when() const noexcept;

    private:
        const exception_item item;  //  Store the exception item.
};

}  //  end namespace wte

#endif
