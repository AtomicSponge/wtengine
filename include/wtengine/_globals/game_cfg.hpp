/*!
 * WTEngine | File:  game_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.2a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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

#include "wtengine/_globals/variable_map.hpp"

namespace wte
{

/*!
 * \class game_cfg
 * \brief Store game config variables in a globally accessible class.
 * 
 * Inherits map functionality from the variable_map class.
 * This class provides specific load/save functions.
 * Also provides math functions to manipulate the values.
 */
class game_cfg final : public variable_map<game_cfg> {
    public:
        /*!
         * \brief Set the file the game config variables will be written to.
         * 
         * \param fname Filename to set saving to.
         */
        inline static void set_game_cfg_file(const std::string& fname) {
            data_file_name = fname;
        };

        /*!
         * \brief Allows a different encryption key to be set.
         * 
         * \param key New key.
         */
        inline static void set_enc_key(const char& key) {
            NOT_THE_ENCRYPTION_KEY = key;
        };

        /*!
         * \brief Load game config variables from file.
         * 
         * \return False on fail, true on success.
         */
        inline static bool load(void) {
            std::ifstream data_file(data_file_name);
            if(!data_file.good()) return false;

            std::string it;
            //  Read each line, try to register or set
            while(std::getline(data_file, it)) {
                it = decrypt(it);
                if(!reg(it)) set(it);
            }

            data_file.close();
            return true;
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
        inline static bool save(const std::string& var) {
            std::ofstream data_file(data_file_name, std::ofstream::app);
            if(!data_file.good()) return false;

            try {
                data_file << encrypt(var + "=" + _map.at(var)) << std::endl;
            } catch (std::out_of_range& e) {
                data_file.close();
                return false;  //  Didn't find var
            }

            data_file.close();
            return true;
        };

        /*!
         * \brief Add a value to a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to add by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool add(const std::string& var, const T& val) {
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
         * \brief Subtract a value from a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to subtract by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool subtract(const std::string& var, const T& val) {
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
         * \brief Multiply a value by a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to multiply by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool multiply(const std::string& var, const T& val) {
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
         * \brief Divide a value by a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to divide by.
         * \return False on fail, true on success.
         */
        template <typename T> inline static bool divide(const std::string& var, const T& val) {
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
         * 
         * Call this first before accessing.
         * 
         * \param var Variable name.
         * \param val Initial value of variable.
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& var, const std::string& val) {
            return variable_map<game_cfg>::reg(var, val);
        };

        /*!
         * \brief Create a new entry in the map.
         * 
         * Call this first before accessing.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if created, false if not created.
         */
        inline static const bool reg(const std::string& expr) {
            return variable_map<game_cfg>::reg(expr);
        };

        /*!
         * \brief Check if a variable is registered in the map.
         * 
         * \param var Variable name to check.
         * \return True if it exists, false if it does not.
         */
        inline static const bool is_reg(const std::string& var) {
            return variable_map<game_cfg>::is_reg(var);
        }

        /*!
         * \brief Set key to value.
         * 
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& var, const std::string& val) {
            return variable_map<game_cfg>::set(var, val);
        };

        /*!
         * \brief Set a value based on string.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& expr) {
            return variable_map<game_cfg>::set(expr);
        };

        /*!
         * \brief Get a value.
         * 
         * Returns by string.
         * 
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        inline static const std::string get(const std::string& var) {
            return variable_map<game_cfg>::get(var);
        };

        /*!
         * \brief Get a value.
         * 
         * Returns by defined type.
         * 
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> inline static const T get(const std::string& var) {
            return variable_map<game_cfg>::get<T>(var);
        };

    private:
        /*!
         * \brief Encrypt string.
         * 
         * \param input String to encrypt.
         * \return Encrypted string.
         */
        inline static const std::string encrypt(std::string input) {
            for(std::size_t i = 0; i < input.length(); i++) {
                input[i] = input[i] - NOT_THE_ENCRYPTION_KEY;
            }

            return input;
        };

        /*!
         * \brief Decrypt string.
         * 
         * \param input String to decrypt.
         * \return Decrypted string.
         */
        inline static const std::string decrypt(std::string input) {
            for(std::size_t i = 0; i < input.length(); i++) {
                input[i] = input[i] + NOT_THE_ENCRYPTION_KEY;
            }

            return input;
        };

        inline static std::string data_file_name = "game.cfg";
        inline static char NOT_THE_ENCRYPTION_KEY = '@';
};

//!  Declare a copy of the variable map for game_cfg.
template <> inline std::map<std::string, std::string> variable_map<game_cfg>::_map = {};

} //  end namespace wte

#endif
