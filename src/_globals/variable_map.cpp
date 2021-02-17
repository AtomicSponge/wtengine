/*!
 * WTEngine | File:  variable_map.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <limits>
#include <any>

#include "wtengine/variable_map.hpp"

namespace wte
{

static const bool variable_map::reg(const std::string& var, const std::string& val) {
    if(var.find('=') != std::string::npos)
        return false;
    if(val.find('=') != std::string::npos)
        return false;
    auto ret = _map.insert(std::make_pair(var, val));
    if(ret.second == false) return false;  //  Key exists already
    else return true;  //  Inserted new key/pair
}

static const bool variable_map::reg(const std::string& expr) {
    std::string var = expr.substr(0, expr.find("="));
    std::string val = expr.substr(expr.find("=") + 1, expr.length());

    return reg(var, val);
}

static const bool variable_map::is_reg(const std::string& var) {
    try {
        _map.at(var);
        return true;
    } catch (std::out_of_range& e) {
        return false;  //  Didn't find var
    }
}

static const bool variable_map::set(const std::string& var, const std::string& val) {
    try {
        _map.at(var) = val;
        return true;
    } catch (std::out_of_range& e) {
        return false;  //  Didn't find var
    }
}

static const bool variable_map::set(const std::string& expr) {
    std::string var = expr.substr(0, expr.find("="));
    std::string val = expr.substr(expr.find("=") + 1, expr.length());

    return set(var, val);
}

static const std::string variable_map::get(const std::string& var) {
    try {
        return _map.at(var);
    } catch (std::out_of_range& e) {
        return nullptr;  //  Didn't find
    }
}

template <typename T> static const T variable_map::get(const std::string& var) {
    try {
        T temp;
        std::stringstream(_map.at(var)) >>
            std::setprecision(std::numeric_limits<T>::max_digits10) >> std::fixed >> temp;
        return std::any_cast<const T>(temp);
    } catch (std::out_of_range& e) {
        return std::any_cast<const bool>(false);  //  Didn't find
    } catch (std::bad_any_cast& e) {
        return std::any_cast<const bool>(false);  //  Bad cast
    }
}

} //  end namespace wte
