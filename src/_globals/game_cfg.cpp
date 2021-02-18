/*!
 * WTEngine | File:  game_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <any>
#include <stdexcept>

#include "wtengine/_globals/game_cfg.hpp"

namespace wte
{

void game_cfg::set_game_cfg_file(const std::string& fname) {
    data_file_name = fname;
}

void game_cfg::set_enc_key(const char& key) {
    NOT_THE_ENCRYPTION_KEY = key;
}

bool game_cfg::load(void) {
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
}

void game_cfg::clear_save(void) {
    std::ofstream data_file(data_file_name, std::ofstream::trunc);
    data_file.close();
}

bool game_cfg::save(const std::string& var) {
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
}

template <typename T> bool game_cfg::add(const std::string& var, const T& val) {
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
}

template <typename T> bool game_cfg::subtract(const std::string& var, const T& val) {
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
}

template <typename T> bool game_cfg::multiply(const std::string& var, const T& val) {
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
}

template <typename T> bool game_cfg::divide(const std::string& var, const T& val) {
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
}

const bool game_cfg::reg(const std::string& var, const std::string& val) {
    return variable_map<game_cfg>::reg(var, val);
}

const bool game_cfg::reg(const std::string& expr) {
    return variable_map<game_cfg>::reg(expr);
}

const bool game_cfg::is_reg(const std::string& var) {
    return variable_map<game_cfg>::is_reg(var);
}

const bool game_cfg::set(const std::string& var, const std::string& val) {
    return variable_map<game_cfg>::set(var, val);
}

const bool game_cfg::set(const std::string& expr) {
    return variable_map<game_cfg>::set(expr);
}

const std::string game_cfg::get(const std::string& var) {
    return variable_map<game_cfg>::get(var);
}

template <typename T> const T game_cfg::get(const std::string& var) {
    return variable_map<game_cfg>::get<T>(var);
}

const std::string game_cfg::encrypt(std::string input) {
    for(std::size_t i = 0; i < input.length(); i++) {
        input[i] = input[i] - NOT_THE_ENCRYPTION_KEY;
    }

    return input;
}

const std::string game_cfg::decrypt(std::string input) {
    for(std::size_t i = 0; i < input.length(); i++) {
        input[i] = input[i] + NOT_THE_ENCRYPTION_KEY;
    }

    return input;
}

} //  end namespace wte
