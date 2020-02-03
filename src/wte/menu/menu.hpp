/*
  WTEngine
  By:  Matthew Evans
  File:  menu.hpp

  See LICENSE.txt for copyright information

  A menu
*/

#ifndef WTE_MENU_MENU_HPP
#define WTE_MENU_MENU_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

typedef std::vector<menu_item_sptr> menu_items;
typedef std::vector<menu_item_sptr>::iterator menu_item_iterator;
typedef std::vector<menu_item_sptr>::const_iterator menu_item_citerator;

class menu {
    public:
        inline menu(const std::string i, const int w, const int h, const int p) :
                    id(i), title(""), width(w), height(h), padding(p) {};
        inline menu(const std::string i, const std::string t, const int w, const int h, const int p) :
                    id(i), title(t), width(w), height(h), padding(p) {};
        inline ~menu() {};

        inline const std::string get_id(void) const { return id; }
        inline const std::string get_title(void) const { return title; }
        inline const int get_width(void) const { return width; }
        inline const int get_height(void) const { return height; }
        inline const int get_padding(void) const { return padding; }
        inline menu_items get_items(void) const { return items; }

        inline void set_title(const std::string t) { title = t; }

        const bool add_item(const menu_item);

    private:
        menu_items items;

        std::string id;
        std::string title;

        int width, height, padding;
};

//!  Add a menu item to an existing menu
/*!
*/
inline const bool menu::add_item(const menu_item item) {
    items.push_back(std::make_shared<menu_item>(item));
    return true;
}

}  // end namespace mnu

}  // end namespace wte

#endif
