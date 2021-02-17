/*!
 * WTEngine | File:  engine_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <fstream>

#include "wtengine/engine_cfg.hpp"

namespace wte
{

static bool engine_cfg::load(void) {
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
}

static bool engine_cfg::save(void) {
    std::ofstream data_file("settings.cfg");
    if(!data_file.good()) return false;

    //  Write each pair to file
    for(auto it = _map.cbegin(); it != _map.cend(); it++) {
        data_file << it->first << "=" << it->second << std::endl;
    }

    data_file.close();
    return true;
}

static const bool engine_cfg::reg(const std::string& var, const std::string& val) {
    return variable_map<engine_cfg>::reg(var, val);
}

static const bool engine_cfg::reg(const std::string& expr) {
    return variable_map<engine_cfg>::reg(expr);
}

static const bool engine_cfg::is_reg(const std::string& var) {
    return variable_map<engine_cfg>::is_reg(var);
}

static const bool engine_cfg::set(const std::string& var, const std::string& val) {
    return variable_map<engine_cfg>::set(var, val);
}

static const bool engine_cfg::set(const std::string& expr) {
    return variable_map<engine_cfg>::set(expr);
}

static const std::string engine_cfg::get(const std::string& var) {
    return variable_map<engine_cfg>::get(var);
}

template <typename T> static const T engine_cfg::get(const std::string& var) {
    return variable_map<engine_cfg>::get<T>(var);
}

} //  end namespace wte
