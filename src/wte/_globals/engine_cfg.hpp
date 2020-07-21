/*!
 * WTEngine | File:  engine_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_ENGINE_CFG_HPP
#define WTE_ENGINE_CFG_HPP

#include <string>
#include <fstream>

#include "variable_map.hpp"

namespace wte
{

/*!
 * \class engine_cfg
 * \brief Statics to store engine config variables.
 * 
 * Inherits map functionality from the variable_map class.
 * This class provides load/save functions.
 */
class engine_cfg final : public variable_map<engine_cfg> {
    public:
        /*!
         * \brief Load engine config settings from file.
         * 
         * Will create the config variable if it does not exist.
         * If it does exist, the value is updated from the file.
         * 
         * \return False on fail, true on success.
         */
        inline static bool load(void) {
            std::ifstream data_file("settings.cfg");
            //  Data file doesn't exist, create one
            if(!data_file.good()) {
                save();
                data_file.open("settings.cfg");
            }
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
         * \brief Save engine config settings to file.
         * 
         * \return False on fail, true on success.
         */
        inline static bool save(void) {
            std::ofstream data_file("settings.cfg");
            if(!data_file.good()) return false;

            //  Write each pair to file
            for(auto it = _map.cbegin(); it != _map.cend(); it++) {
                data_file << it->first << "=" << it->second << std::endl;
            }

            data_file.close();
            return true;
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
            return variable_map<engine_cfg>::reg(var, val);
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
            return variable_map<engine_cfg>::reg(expr);
        };

        /*!
         * \brief Check if a variable is registered in the map.
         * 
         * \param var Variable name to check.
         * \return True if it exists, false if it does not.
         */
        inline static const bool is_reg(const std::string& var) {
            return variable_map<engine_cfg>::is_reg(var);
        }

        /*!
         * \brief Set key to value.
         * 
         * \param var Variable name to set.
         * \param val Value to set.
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& var, const std::string& val) {
            return variable_map<engine_cfg>::set(var, val);
        };

        /*!
         * \brief Set based on string.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        inline static const bool set(const std::string& expr) {
            return variable_map<engine_cfg>::set(expr);
        };

        /*!
         * \brief Get value.
         * 
         * Returns by string.
         * 
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        inline static const std::string get(const std::string& var) {
            return variable_map<engine_cfg>::get(var);
        };

        /*!
         * \brief Get value.
         * 
         * Returns by defined type.
         * 
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> inline static const T get(const std::string& var) {
            return variable_map<engine_cfg>::get<T>(var);
        };
};

//!  Declare a copy of the variable map for engine_cfg.
template <> inline std::map<std::string, std::string> variable_map<engine_cfg>::_map = {};

} //  end namespace wte

#endif
