/*!
 * wtengine | File:  alert.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */


#include "wtengine/_globals/alert.hpp"

namespace wte {

std::string alert::alert_text;
std::string alert::alert_location;
int64_t alert::alert_time = -1;
bool alert::alert_set = false;

/*
 * 
 */
void alert::set(
    const std::string& a,
    const std::string& l,
    const int64_t& t
) {
    alert_text = a;
    alert_location = l;
    alert_time = t;
    alert_set = true;
}

/*
 * 
 */
const std::string alert::get(void) { 
    return "Alert: " + alert_text + " At: " + alert_location + "Time: " + std::to_string(alert_time);
}

/*
 * 
 */
const std::string alert::get_text(void) { return alert_text; }

/*
 * 
 */
const std::string alert::get_location(void) { return alert_location; }

/*
 * 
 */
const int64_t alert::get_time(void) { return alert_time; }

/*
 * 
 */
const bool alert::is_set(void) { return alert_set; }

/*
 * 
 */
void alert::clear(void) { alert_set = false; }

}  //  end namespace wte
