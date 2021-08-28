/*!
 * WTEngine | File:  notice.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */


#include "wtengine/_globals/notice.hpp"

namespace wte {

std::string notice::notice_text;
bool notice::notice_set = false;

/*
 * 
 */
void notice::set(
    const std::string& n
) {
    notice_text = n;
    notice_set = true;
};

/*
 * 
 */
const std::string notice::get(void) {  return notice_text; };

/*
 * 
 */
const bool notice::is_set(void) { return notice_set; };

/*
 * 
 */
void notice::clear(void) { notice_set = false; };

}  //  end namespace wte
