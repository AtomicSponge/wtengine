/*!
 * WTEngine | File:  variables.hpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_VARIABLES_HPP
#define WTE_MGR_VARIABLES_HPP

#include <string>
#include <cstring>
#include <any>
#include <map>
#include <fstream>
#include <typeinfo>
#include <type_traits>

#include <iostream>

#include "wtengine/mgr/manager.hpp"
#include "wtengine/_globals/wte_exception.hpp"

namespace wte
{

namespace mgr
{

/*!
 * \class variables
 * \brief Store game variables
 */
class variables final : private manager<variables> {
    public:
        /*!
         * \brief Set the file the game config variables will be written to.
         * 
         * \param fname Filename to set saving to.
         */
        inline static void set_data_file(const std::string& fname) {
            data_file_name = fname;
        };

        /*!
         * \brief Clear the current game config save.
         */
        inline static void clear_save(void) {
            std::ofstream data_file(data_file_name, std::ofstream::trunc);
            data_file.close();
        };

        /*!
         * \brief Save a game config variable to file.
         * 
         * \param var Variable name.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool save(const std::string& var) {
            verify<T>();
            if(isreg(var)) {
                std::ofstream dfile(data_file_name, std::ios::binary | std::ofstream::app);
                if(!dfile.good()) return false;

                try {
                    T tempv = get<T>(var);

                    {int32_t tempi = std::strlen(var.c_str()) + 1;
                    dfile.write(reinterpret_cast<const char*>(&tempi), sizeof(int32_t));
                    dfile.write(var.c_str(), tempi);}

                    {int32_t tempi = sizeof(T);
                    dfile.write(reinterpret_cast<const char*>(&tempi), sizeof(int32_t));
                    dfile.write(reinterpret_cast<const char*>(&tempv), tempi);}
                } catch(...) {
                    dfile.close();
                    return false;
                }

                dfile.close();
                return true;
            }
            return false;
        };

        /*!
         * \brief Load game config variables from file.
         *
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool load(const std::string& var) {
            verify<T>();
            std::ifstream dfile(data_file_name, std::ios::binary);
            if(!dfile.good()) return false;
            dfile.seekg(0, dfile.beg);

            try {
                while(true) {
                    if(dfile.peek() == EOF) return false;
                    std::string in_var;

                    {int32_t size;
                    dfile.read(reinterpret_cast<char*>(&size), sizeof(int32_t));
                    char* buffer = new char[size];
                    dfile.read(buffer, size);
                    in_var = std::string(buffer);
                    delete[] buffer;}

                    int32_t size;
                    dfile.read(reinterpret_cast<char*>(&size), sizeof(int32_t));
                    if(var == in_var) {
                        T in_val;
                        dfile.read(reinterpret_cast<char*>(&in_val), size);
                        set<T>(var, in_val);
                        break;
                    }
                    dfile.seekg(size, dfile.cur);
                }
            } catch(...) {
                dfile.close();
                return false;
            }

            dfile.close();
            return true;
        };

        /*!
         * Create a new entry in the map.
         * Call this first before accessing.
         */
        template <typename T> inline static const bool reg(const std::string& var, const T& val) {
            verify<T>();
            auto ret = _map.insert(std::make_pair(var, std::make_any<T>(val)));
            return ret.second;
        };

        /*!
         * Check if registered
         */
        inline static const bool isreg(const std::string& var) {
            auto it = _map.find(var);
            if(it != _map.end()) return true;
            return false;
        };

        /*!
         * Delete entry
         */
        inline static const bool del(const std::string& var) {
            auto it = _map.find(var);
            if(it != _map.end()) {
                _map.erase(it);
                return true;
            }
            return false;
        };

        /*!
         * Set key
         * Will override value
         */
        template <typename T> inline static void set(const std::string& var, const T& val) {
            verify<T>();
            try {
                _map.at(var) = std::make_any<T>(val);
            } catch(std::out_of_range& e) {
                std::string err_msg = "Could not set variable: " + var;
                throw wte_exception(err_msg.c_str());
            }
        };

        /*!
         * Get value
         */
        template <typename T> inline static const T get(const std::string& var) {
            verify<T>();
            try {
                return std::any_cast<const T>(_map.at(var));
            } catch(std::out_of_range& e) {
                std::string err_msg = "Could not get variable: " + var;
                throw wte_exception(err_msg.c_str());
            } catch(std::bad_any_cast& e) {
                std::string err_msg = "Error reading variable: " + var;
                throw wte_exception(err_msg.c_str());
            }
        };

    private:
        inline variables() { _map.clear(); };
        inline ~variables() { _map.clear(); };

        /*!
         * Make sure valid data type
         */
        template <typename T> inline static void verify(void) {
            static_assert(
                std::is_same<int, T>::value ||
                std::is_same<int16_t, T>::value ||
                std::is_same<int32_t, T>::value ||
                std::is_same<int64_t, T>::value ||
                std::is_same<uint, T>::value ||
                std::is_same<uint16_t, T>::value ||
                std::is_same<uint32_t, T>::value ||
                std::is_same<uint64_t, T>::value ||
                std::is_same<float, T>::value ||
                std::is_same<double, T>::value ||
                std::is_same<long double, T>::value
            );
        };

        inline static std::string data_file_name = "game.cfg";

        inline static std::map<std::string, std::any> _map = {};
};

template <> inline bool manager<variables>::initialized = false;

} //  end namespace mgr

} //  end namespace wte

#endif
