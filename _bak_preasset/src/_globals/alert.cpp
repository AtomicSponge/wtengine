/*!
 * WTEngine | File:  alert.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */


#include "wtengine/_globals/alert.hpp"

namespace wte
{

ALLEGRO_COLOR alert::font_color = {};
ALLEGRO_COLOR alert::bg_color = {};
std::string alert::alert_text = "";
std::string alert::alert_location = "";
int64_t alert::alert_time = -1;
bool alert::alert_status = false;
bool alert::alert_set = false;

alert::alert() {}
alert::~alert() {}

/*
 * 
 */
void alert::set(
    const std::string& a,
    const std::string& l,
    const int64_t& t,
    const bool& s
) {
    alert_text = a;
    alert_location = l;
    alert_time = t;
    alert_status = s;
    alert_set = true;
};

/*
 * 
 */
const std::string alert::get(void) { return alert_text; };

/*
 * 
 */
const std::string alert::get_location(void) { return alert_location; };

/*
 * 
 */
const int64_t alert::get_time(void) { return alert_time; };

/*
 * 
 */
const bool alert::get_status(void) { return alert_status; };

/*
 * 
 */
const bool alert::is_set(void) { return alert_set; };

/*
 * 
 */
const ALLEGRO_COLOR alert::get_font_color(void) { return font_color; };

/*
 * 
 */
const ALLEGRO_COLOR alert::get_bg_color(void) { return bg_color; };

/*
 * 
 */
void alert::set_font_color(const ALLEGRO_COLOR& c) { font_color = c; };

/*
 * 
 */
void alert::set_bg_color(const ALLEGRO_COLOR& c) { bg_color = c; };

/*
 * 
 */
void alert::clear(void) { alert_set = false; };

}  //  end namespace wte
