/*!
 * WTEngine | File:  variables.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/variables.hpp"

namespace wte
{

namespace mgr
{

template <> bool variables::manager<variables>::initialized = false;

std::string variables::data_file_name = "game.cfg";
std::map<std::string, std::any> variables::_map = {};

/*
 *
 */
void variables::set_data_file(const std::string& fname) {
    data_file_name = fname;
}

/*
 *
 */
bool variables::clear_save(void) {
    std::ofstream dfile(data_file_name, std::ofstream::trunc);
    if(!dfile.good()) return false;
    dfile.close();
    return true;
}

/*
 *
 */
const bool variables::isreg(const std::string& var) {
    auto it = _map.find(var);
    if(it != _map.end()) return true;
    return false;
}

/*
 *
 */
const bool variables::del(const std::string& var) {
    auto it = _map.find(var);
    if(it != _map.end()) {
        _map.erase(it);
        return true;
    }
    return false;
}

} //  end namespace mgr

} //  end namespace wte
