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
#include <any>
#include <map>

namespace wte
{

//!  Variable map
/*!
 * Create a map of variables
 */
template <typename derived> class variable_map {
    public:
        //!  Remove copy constructor
        variable_map(const variable_map&) = delete;
        //!  Remove assignment operator
        void operator=(variable_map const&) = delete;

        /*!
         *
         */
        template <class T>  inline static const bool reg(const std::string var, const T val) {
            auto ret = _map.insert(std::make_pair(var, val));
            if(ret.second == false) return false;  //  Key exists already
            else return true;  //  Inserted new key/pair
        };

        /*!
         *
         */
        inline static void check_type(const std::string var) {
            //
        };

        /*!
         *
         */
        inline static void set(const std::string expr) {
            set<int>(expr, 0);
        };

        /*!
         *
         */
        template <class T> inline static void set(const std::string var, const T val) {
            try {
                _map.at(var) = val;
            } catch (...) {
                //  Didn't find var
            }
        };

        /*!
         *
         */
        template <class T> inline static const T get(const std::string var) {
            try {
                return std::any_cast<T>(_map.at(var));
            } catch (std::out_of_range& e) {
                std::exit(0);  //  Didn't find
            } catch (std::bad_any_cast& e) {
                std::exit(0);  //  Bad cast
            }
        };

    private:
        //inline variable_map() {};
        //inline ~variable_map() {};

        static std::map<std::string, std::any> _map;
};

//typedef variable_map<engine_stuffs> engine_stuffs;

} //  end namespace wte

#endif
