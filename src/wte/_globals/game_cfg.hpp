/*!
 * \brief WTEngine | File:  game_cfg.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Game config.
 */

#ifndef WTE_GAME_CFG_HPP
#define WTE_GAME_CFG_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <any>
#include <stdexcept>

#include "variable_map.hpp"

namespace wte
{

/*!
 * \class Game config map
 * Statics to store game config variables.
 * Inherits map functionality from the variable_map class.
 * This class provides load/save functions.
 * Also provides math functions to manipulate the values.
 */
class game_cfg final : public variable_map<game_cfg> {
    public:
        /*!
         * Set the file the game config variables will be written to.
         * \param fname Filename to set saving to.
         * \return void
         */
        inline static void set_game_cfg_file(const std::string& fname) {
            data_file_name = fname;
        }

        /*!
         * Load game config variables from file.
         * \param void
         * \return False on fail, true on success.
         */
        inline static bool load(void) {
            std::ifstream data_file(data_file_name);
            if(!data_file.good()) return false;

            std::string it;
            //  Read each line, try to register or set
            while(std::getline(data_file, it)) {
                if(!reg(it)) set(it);
            }

            data_file.close();
            return true;
        };

        /*!
         * Clear the current game config save.
         * \param void
         * \return void
         */
        inline static void clear_save(void) {
            std::ofstream data_file(data_file_name, std::ofstream::trunc);
            data_file.close();
        };

        /*!
         * Save a game config variable to file.
         * \param var Variable name.
         * \return False on fail, true on success.
         */
        inline static bool save(const std::string& var) {
            std::ofstream data_file(data_file_name, std::ofstream::app);
            if(!data_file.good()) return false;

            try {
                data_file << var << "=" << _map.at(var) << std::endl;
            } catch (std::out_of_range& e) {
                data_file.close();
                return false;  //  Didn't find var
            }

            data_file.close();
            return true;
        };

        /*!
         * Add a value to a variable in the game config map.
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to add by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool add(const std::string& var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp += val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Subtract a value from a variable in the game config map.
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to subtract by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool subtract(const std::string& var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp -= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Multiply a value by a variable in the game config map.
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to multiply by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool multiply(const std::string& var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp *= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * Divide a value by a variable in the game config map.
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to divide by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool divide(const std::string& var, T val) {
            try {
                T temp;
                std::stringstream(_map.at(var)) >>
                    std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
                _map.at(var) = std::to_string(std::any_cast<T>(temp /= val));
                return true;
            } catch (std::out_of_range& e) {
                return false;  //  Didn't find
            } catch (std::bad_any_cast& e) {
                return false;  //  Bad cast
            }
        };

        /*!
         * \brief Create a new entry in the map.
         * Call this first before accessing.
         * \param var Variable name.
         * \param val Initial value of variable.
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& var, const std::string& val) {
            return variable_map<game_cfg>::reg(var, val);
        };

        /*!
         * \brief Create a new entry in the map.
         * Call this first before accessing.
         * \param expr Variable and value expresion (var=val)
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& expr) {
            return variable_map<game_cfg>::reg(expr);
        };

        /*!
         * Check if a variable is registered in the map.
         * \param var Variable name to check.
         * \return True if it exists, false if it does not.
         */
        inline static const bool is_reg(const std::string& var) {
            return variable_map<game_cfg>::is_reg(var);
        }

        /*!
         * Set key to value.
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& var, const std::string& val) {
            return variable_map<game_cfg>::set(var, val);
        };

        /*!
         * Set based on string
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& expr) {
            return variable_map<game_cfg>::set(expr);
        };

        /*!
         * \brief Get value.
         * Returns by string.
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        inline static const std::string get(const std::string& var) {
            return variable_map<game_cfg>::get(var);
        };

        /*!
         * \brief Get value.
         * Returns by defined type.
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> inline static const T get(const std::string& var) {
            return variable_map<game_cfg>::get<T>(var);
        };

    private:
        inline static std::string data_file_name = "game.cfg";
};

template <> inline std::map<std::string, std::string> variable_map<game_cfg>::_map = {};

} //  end namespace wte

#endif
