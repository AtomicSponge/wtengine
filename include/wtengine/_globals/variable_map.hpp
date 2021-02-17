/*!
 * WTEngine | File:  variable_map.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_VARIABLE_MAP_HPP
#define WTE_VARIABLE_MAP_HPP

#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <limits>
#include <any>

namespace wte
{

/*!
 * \class variable_map
 * \brief Interface class to create a map of variables.
 */
template <class derived> class variable_map {
    public:
        //!  Remove copy constructor
        variable_map(const variable_map&) = delete;
        //!  Remove assignment operator
        void operator=(variable_map const&) = delete;

        /*!
         * \brief Create a new entry in the map.
         * 
         * Call this first before accessing the variable.
         * 
         * \param var Variable name.
         * \param val Initial value of variable.
         * \return True if created, false if not created.
         */
        static const bool reg(const std::string& var, const std::string& val);

        /*!
         * \brief Create a new entry in the map.
         * 
         * Call this first before accessing the variable.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if created, false if not created.
         */
        static const bool reg(const std::string& expr);

        /*!
         * \brief Check if a variable is registered in the map.
         * 
         * \param var Variable name to check.
         * \return True if it exists, false if it does not.
         */
        static const bool is_reg(const std::string& var);

        /*!
         * \brief Set a variable to a value.
         * 
         * Passes seperate strings for the variable name and value.
         * 
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        static const bool set(const std::string& var, const std::string& val);

        /*!
         * \brief Set a variable to a value.
         * 
         * Passes a single string as an expression var=val.
         * 
         * \param expr Variable and value expresion.
         * \return True if set, false if not set.
         */
        static const bool set(const std::string& expr);

        /*!
         * \brief Get a variable's value.
         * 
         * Returns by string.
         * 
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        static const std::string get(const std::string& var);

        /*!
         * \brief Get a variable's value.
         * 
         * Returns by casted type.
         * 
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> static const T get(const std::string& var);

    protected:
        //!  Store the variable map.
        static std::map<std::string, std::string> _map;

    private:
        inline variable_map() { _map.clear(); };
        inline ~variable_map() { _map.clear(); };
};

} //  end namespace wte

#endif
