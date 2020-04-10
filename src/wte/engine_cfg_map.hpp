/*!
 * \brief WTEngine | File:  engine_cfg_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine config.
 */

#ifndef WTE_ENGINE_CFG_MAP_HPP
#define WTE_ENGINE_CFG_MAP_HPP

#include <fstream>
#include <stdexcept>

#include "variable_map.hpp"

namespace wte
{

//!  Engine config
/*!
 * Statics to store engine config variables
 */
class engine_cfg_map final : public variable_map<engine_cfg_map> {
    public:
        /*!
         * Load
         * False on fail, true on success
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
         * Save
         * False on fail, true on success
         */
        inline static bool save(void) {
            std::ofstream data_file("settings.cfg");
            if(!data_file.good()) return false;

            //  Write each pair to file
            for(auto it = _map.begin(); it != _map.end(); it++) {
                data_file << it->first << "=" << it->second << std::endl;
            }

            data_file.close();
            return true;
        };
};

typedef engine_cfg_map::variable_map<engine_cfg_map> engine_cfg;

template <> inline std::map<std::string, std::string> engine_cfg::_map = {};

} //  end namespace wte

#endif
