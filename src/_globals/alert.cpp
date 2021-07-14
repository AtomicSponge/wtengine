/*!
 * WTEngine | File:  alert.cpp
 * 
 * \author Matthew Evans
 * \version 0.4
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */


#include "wtengine/_globals/alert.hpp"

namespace wte
{

ALLEGRO_COLOR alert::font_color = {};
ALLEGRO_COLOR alert::bg_color = {};
std::string alert::alert_text = "";
bool alert::alert_set = false;

/*
 * 
 */
void alert::set_alert(const std::string& a) { set(a); };

/*
 * 
 */
void alert::set(const std::string& a) {
    alert_text = a;
    alert_set = true;
};

/*
 * 
 */
const std::string alert::get_alert(void) { return alert_text; };

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
void alert::set_font_color(const ALLEGRO_COLOR c) { font_color = c; };

/*
 * 
 */
void alert::set_bg_color(const ALLEGRO_COLOR c) { bg_color = c; };

/*
 * 
 */
void alert::clear(void) { alert_set = false; };

}  //  end namespace wte
