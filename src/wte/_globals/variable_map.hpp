/*!
 * \brief WTEngine | File:  variable_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Variable map.
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
 * \brief Interface class to create a map of variables
 */
template <class derived> class variable_map {
    public:
        //!  Remove copy constructor
        variable_map(const variable_map&) = delete;
        //!  Remove assignment operator
        void operator=(variable_map const&) = delete;

        /*!
         * \brief Create a new entry in the map.
         * Call this first before accessing.
         * \param var Variable name.
         * \param val Initial value of variable.
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& var, const std::string& val) {
            if(var.find('=') != std::string::npos)
                return false;
            if(val.find('=') != std::string::npos)
                return false;
            auto ret = _map.insert(std::make_pair(var, val));
            if(ret.second == false) return false;  //  Key exists already
            else return true;  //  Inserted new key/pair
        };

        /*!
         * \brief Create a new entry in the map.
         * Call this first before accessing.
         * \param expr Variable and value expresion (var=val)
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& expr) {
            std::string var = expr.substr(0, expr.find("="));
            std::string val = expr.substr(expr.find("=") + 1, expr.length());

            return reg(var, val);
        };

        /*!
         * Check if a variable is registered in the map.
         * \param var Variable name to check.
         * \return True if it exists, false if it does not.
         */
        inline static const bool is_reg(const std::string& var) {
            try {
                _map.at(var);
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find var
            }
        }

        /*!
         * Set key to value.
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& var, const std::string& val) {
            try {
                _map.at(var) = val;
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find var
            }
        };

        /*!
         * Set based on string
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& expr) {
            std::string var = expr.substr(0, expr.find("="));
            std::string val = expr.substr(expr.find("=") + 1, expr.length());

            return set(var, val);
        };

        /*!
         * \brief Get value.
         * Returns by string.
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        inline static const std::string get(const std::string& var) {
            try {
                return _map.at(var);
            } catch (std::out_of_range& e) {
                return nullptr;  //  Didn't find
            }
        };

        /*!
         * \brief Get value.
         * Returns by defined type.
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> inline static const T get(const std::string& var) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                return std::any_cast<const T>(temp);
            } catch (std::out_of_range& e) {
                return std::any_cast<const bool>(false);  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return std::any_cast<const bool>(false);  //  Bad cast
            }
        };

    protected:
        //!  Store the variable map.
        static std::map<std::string, std::string> _map;

    private:
        inline variable_map() { _map.clear(); };
        inline ~variable_map() { _map.clear(); };
};

} //  end namespace wte

#endif
