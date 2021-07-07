/*!
 * WTEngine | File:  config.cpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/config.hpp"

namespace wte
{

/*
 *
 */
bool config::load(void) {
    /*std::ifstream data_file("settings.cfg");
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

    data_file.close();*/
    return true;
}

/*
 *
 */
bool config::save(void) {
    /*std::ofstream data_file("settings.cfg");
    if(!data_file.good()) return false;

    //  Write each pair to file
    for(auto it = _map.cbegin(); it != _map.cend(); it++) {
        data_file << it->first << "=" << it->second << std::endl;
    }

    data_file.close();*/
    return true;
}

} //  end namespace wte
