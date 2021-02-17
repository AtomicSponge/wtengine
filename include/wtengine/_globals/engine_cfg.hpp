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

#include "wtengine/_globals/variable_map.hpp"

namespace wte
{

/*!
 * \class engine_cfg
 * \brief Store engine config variables in a globally accessible class.
 * 
 * Inherits map functionality from the variable_map class.
 * This class provides specific load/save functions.
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
        static bool load(void);

        /*!
         * \brief Save engine config settings to file.
         * 
         * \return False on fail, true on success.
         */
        static bool save(void);
        
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
         * \brief Set based on string.
         * 
         * \param expr Variable and value expresion (var=val)
         * \return True if set, false if not set.
         */
        static const bool set(const std::string& expr);

        /*!
         * \brief Get value.
         * 
         * Returns by string.
         * 
         * \param var Variable to get.
         * \return Value of variable in string form.
         */
        static const std::string get(const std::string& var);

        /*!
         * \brief Get value.
         * 
         * Returns by defined type.
         * 
         * \tparam T Data type to cast to.
         * \param var Variable to get.
         * \return Value of variable in casted form.
         */
        template <typename T> static const T get(const std::string& var);
};

//!  Declare a copy of the variable map for engine_cfg.
template <> std::map<std::string, std::string> variable_map<engine_cfg>::_map = {};

} //  end namespace wte

#endif
