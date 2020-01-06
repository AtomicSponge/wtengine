/*
  Menu
  By:  Matthew Evans
  File:  menu.hpp

  See LICENSE.txt for copyright information

  A menu
*/

#ifndef WTE_MENU_MENU_HPP
#define WTE_MENU_MENU_HPP

#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

class menu {
    public:
        menu();
        menu(const std::string, const int, const int);
        menu(const menu&);
        ~menu();

        const int get_width(void) const;
        const int get_height(void) const;

    private:
        std::string menu_name;
        std::vector<menu_item> items;
        ALLEGRO_BITMAP *background_bitmap;
};

//!  Default constructor
/*!
*/
inline menu::menu() {
    background_bitmap = NULL;
}

//!  Menu constructor
/*!
*/
inline menu::menu(const std::string name, const int w, const int h) {
    menu_name = name;
    background_bitmap = al_create_bitmap(w, h);
}

//!  Copy constructor
/*!
*/
inline menu::menu(const menu& copy_menu) {
    menu_name = copy_menu.menu_name;
    background_bitmap = al_clone_bitmap(copy_menu.background_bitmap);
}

//!
/*!
*/
inline menu::~menu() {
    al_destroy_bitmap(background_bitmap);
}

//!
/*!
*/
inline const int menu::get_width(void) const { return al_get_bitmap_width(background_bitmap); }

//!
/*!
*/
inline const int menu::get_height(void) const { return al_get_bitmap_height(background_bitmap); }

}  // end namespace mnu

}  // end namespace wte

#endif
