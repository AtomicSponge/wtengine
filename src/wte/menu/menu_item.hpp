/*
  Menu item
  By:  Matthew Evans
  File:  menu_item.hpp

  See LICENSE.txt for copyright information

  A menu item
*/

#ifndef WTE_MENU_MENU_ITEM_HPP
#define WTE_MENU_MENU_ITEM_HPP

#include <string>
#include <vector>

#include "../message.hpp"

namespace wte
{

namespace mnu
{

typedef std::pair<std::string, std::string> item_option;

typedef std::vector<std::pair<std::string, std::string>>::iterator option_iterator;
typedef std::vector<std::pair<std::string, std::string>>::const_iterator option_citerator;

class menu_item {
    public:
        menu_item();
        ~menu_item();

        menu_item(std::string);

        void add_option(const std::string, const std::string);
        const std::string get_label(void) const;

    private:
        std::string label;
        std::vector<item_option> options;

        //  Used for generating the system message:
        std::string sys;
        std::string cmd;
        std::string args;
};

//!
/*!
*/
inline menu_item::menu_item() {}

//!
/*!
*/
inline menu_item::menu_item(std::string l) : label(l) {
    sys = "system";
}

//!
/*!
*/
inline menu_item::~menu_item() {
    //
}

//!
/*!
*/
inline void menu_item::add_option(const std::string option_label, const std::string option_args) {
    options.push_back(std::make_pair(option_label, option_args));
}

//!
/*!
*/
inline const std::string menu_item::get_label(void) const { return label; }

}  // end namespace mnu

}  // end namespace wte

#endif
