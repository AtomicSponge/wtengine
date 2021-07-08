/*!
 * WTEngine | File:  variables.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_VARIABLES_HPP
#define WTE_MGR_VARIABLES_HPP

#include <string>
#include <any>
#include <map>
#include <fstream>
#include <typeinfo>

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
         * \brief Allows a different encryption key to be set.
         * 
         * \param key New key.
         */
        inline static void set_enc_key(const char& key) {
            NOT_THE_ENCRYPTION_KEY = key;
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
            if(isreg(var)) {
                std::ofstream dfile(data_file_name, std::ofstream::app);
                if(!dfile.good()) return false;

                try {
                    dfile.write(reinterpret_cast<const char*>(sizeof(var.c_str())), 32);
                    dfile.write(var.c_str(), sizeof(var.c_str()));
                    dfile.write(reinterpret_cast<const char*>(sizeof(T)), 32);
                    dfile.write(
                        reinterpret_cast<const char*>(std::any_cast<T>((*_map.find(var)).second)),
                        sizeof(T)
                    );
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
            std::ifstream dfile(data_file_name);
            if(!dfile.good()) return false;

            //  Load
            //dfile.read(buffer, sizeof T);

            dfile.close();
            return true;
        };

        /*!
         * Create a new entry in the map.
         * Call this first before accessing.
         */
        template <typename T> inline static const bool reg(const std::string& var, const T& val) {
            auto ret = _map.insert(std::make_pair(var, std::make_any<T>(val)));
            if(ret.second == false) return false;  //  Key exists already
            else return true;  //  Inserted new key/pair
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

        inline static std::map<std::string, std::any> _map = {};
};

template <> inline bool manager<variables>::initialized = false;

} //  end namespace mgr

} //  end namespace wte

#endif
