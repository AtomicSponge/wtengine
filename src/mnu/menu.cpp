/*!
 * WTEngine | File:  menu.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mnu/menu.hpp"

namespace wte::mnu {

/*
 *
 */
menu::menu(
    const std::string& i,
    const std::string& t
) : id(i), title(t) {}

/*
 *
 */
const std::string menu::get_id(void) const { return id; }

/*
 *
 */
const std::string menu::get_title(void) const { return title; }

/*
 *
 */
void menu::set_title(const std::string& t) { title = t; }

/*
 *
 */
const std::size_t menu::num_items(void) const { return _items.size(); }

/*
 *
 */
item_citerator menu::items_cbegin(void) const { return _items.cbegin(); }

/*
 *
 */
item_citerator menu::items_cend(void) const { return _items.cend(); }

/*
 *
 */
const items menu::get_items(void) const { return _items; }

}  //  end namespace wte::mnu
