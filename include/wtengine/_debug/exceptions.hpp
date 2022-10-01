/*!
 * wtengine | File:  exceptions.hpp
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

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/engine_time.hpp"

namespace wte {

const bool logger_add(
    const std::string& d, const std::string& l,
    const uint& c, const int64_t& t);

/*!
 * \class exception_item
 * \brief Creates an info object that can be passed to the engine exceptions.
 */
class exception_item final {
    friend class runtime_error;
    friend class exception;

    private:
        const char* description;  //  Exception description.
        const char* location;     //  Exception location.
        const uint code;          //  Code of error
        const int64_t time;       //  Time of exception.
    
    public:
        /*!
         * \brief Create a new exception item.
         */
        exception_item(const std::string& d, const std::string& l, const uint& c);

        /*!
         * \brief Create a new exception item with the code value of 10.
         */
        exception_item(const std::string& d, const std::string& l);

        /*!
         * \brief Create a new exception item with the code value of 10 and location of Engine.
         */
        exception_item(const std::string& d);
};

/*!
 * \class runtime_error
 * \brief Throws an engine runtime error.
 *
 * Exceptions thrown this way will terminate the engine.
 */
class runtime_error final : public std::exception {
    private:
        const exception_item item;  //  Store the exception item.

    public:
        /*!
         * \brief Create a new runtime error object.
         * \param i An exception item object.
         */
        inline runtime_error(const exception_item& i) : item(i) {};

        /*!
         * \brief Create a new runtime error object.  Sets the location to Engine and code to 1.
         * \param d An exception description.
         */
        inline runtime_error(const std::string& d) : item(exception_item(d.c_str(), "Engine", 1)) {};

        runtime_error() = delete;    //!<  Delete default constructor.

        /*!
         * \brief Terminates engine after handling exception.
         */
        virtual ~runtime_error();

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
};

/*!
 * \class exception
 * \brief Throws an internal engine exception.
 *
 * Exceptions thrown this way will not terminate the engine. \n
 * If debugging is enabled, they will also be logged to file.
 */
class exception final : public std::exception {
    private:
        const exception_item item;  //  Store the exception item.
    
    public:
        /*!
         * \brief Create a wte exception.
         * \param i An exception item object.
         */
        exception(const exception_item& i);

        exception() = delete;            //!<  Delete default constructor.
        virtual ~exception() = default;  //!<  Default virtual destructor.

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
};

}  //  end namespace wte

#endif
