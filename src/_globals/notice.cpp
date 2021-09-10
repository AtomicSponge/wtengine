/*!
 * WTEngine | File:  notice.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */


#include "wtengine/_globals/notice.hpp"

namespace wte {

std::string notice::notice_text;
bool notice::notice_set = false;
wte_asset<al_font> notice::notice_font;
ALLEGRO_COLOR notice::notice_font_color;
ALLEGRO_COLOR notice::notice_bg_color;

/*
 * 
 */
void notice::set(
    const std::string& n
) {
    notice_text = n;
    notice_set = true;
}

/*
 * 
 */
const std::string notice::get(void) {  return notice_text; }

/*
 * 
 */
const bool notice::is_set(void) { return notice_set; }

/*
 * 
 */
void notice::clear(void) { notice_set = false; }

/*
 * 
 */
void notice::set_notice_font(wte_asset<al_font> a) { notice_font = a; }

/*
 * 
 */
const wte_asset<al_font> notice::get_notice_font(void) { return notice_font; }

/*
 * 
 */
void notice::set_notice_font_color(const ALLEGRO_COLOR& c) { notice_font_color = c; }

/*
 * 
 */
const ALLEGRO_COLOR notice::get_notice_font_color(void) { return notice_font_color; }

/*
 * 
 */
void notice::set_notice_bg_color(const ALLEGRO_COLOR& c) { notice_bg_color = c; }

/*
 * 
 */
const ALLEGRO_COLOR notice::get_notice_bg_color(void) { return notice_bg_color; }

}  //  end namespace wte
