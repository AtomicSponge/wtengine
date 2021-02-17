/*!
 * WTEngine | File:  game_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
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
        static void set_game_cfg_file(const std::string& fname);

        /*!
         * \brief Allows a different encryption key to be set.
         * 
         * \param key New key.
         */
        static void set_enc_key(const char& key);

        /*!
         * \brief Load game config variables from file.
         * 
         * \return False on fail, true on success.
         */
        static bool load(void);

        /*!
         * \brief Clear the current game config save.
         */
        static void clear_save(void);

        /*!
         * \brief Save a game config variable to file.
         * 
         * \param var Variable name.
         * \return False on fail, true on success.
         */
        static bool save(const std::string& var);

        /*!
         * \brief Add a value to a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to add by.
         * \return False on fail, true on success.
         */
        template <typename T> static bool add(const std::string& var, const T& val);

        /*!
         * \brief Subtract a value from a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to subtract by.
         * \return False on fail, true on success.
         */
        template <typename T> static bool subtract(const std::string& var, const T& val);

        /*!
         * \brief Multiply a value by a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to multiply by.
         * \return False on fail, true on success.
         */
        template <typename T> static bool multiply(const std::string& var, const T& val);

        /*!
         * \brief Divide a value by a variable in the game config map.
         * 
         * \tparam T Data type to cast and perform calculation in.
         * \param var Variable name.
         * \param val Value to divide by.
         * \return False on fail, true on success.
         */
        template <typename T> static bool divide(const std::string& var, const T& val);

        /*!
         * \brief Create a new entry in the map.
         * 
         * Call this first before accessing.
         * 
         * \param var Variable name.
         * \param val Initial value of variable.
         * \return True if created, false if not created.
         */
        static const bool reg(const std::string& var, const std::string& val);

        /*!
         * \brief Create a new entry in the map.
         * 
         * Call this first before accessing.
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
         * \brief Set key to value.
         * 
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        static const bool set(const std::string& var, const std::string& val);

        /*!
         * \brief Set a value based on string.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        static const bool set(const std::string& expr);

        /*!
         * \brief Get a value.
         * 
         * Returns by string.
         * 
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        static const std::string get(const std::string& var);

        /*!
         * \brief Get a value.
         * 
         * Returns by defined type.
         * 
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> static const T get(const std::string& var);

    private:
        /*!
         * \brief Encrypt string.
         * 
         * \param input String to encrypt.
         * \return Encrypted string.
         */
        static const std::string encrypt(std::string input);

        /*!
         * \brief Decrypt string.
         * 
         * \param input String to decrypt.
         * \return Decrypted string.
         */
        static const std::string decrypt(std::string input);

        inline static std::string data_file_name = "game.cfg";
        inline static char NOT_THE_ENCRYPTION_KEY = '@';
};

//!  Declare a copy of the variable map for game_cfg.
template <> std::map<std::string, std::string> variable_map<game_cfg>::_map = {};

} //  end namespace wte

#endif
