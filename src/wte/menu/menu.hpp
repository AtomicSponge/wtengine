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

#include "../wte_globals.hpp"
#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

class menu {
    public:
        menu();
        menu(const std::string, const int, const int, const ALLEGRO_COLOR);
        menu(const menu&);
        ~menu();

        void set_bg_color(const ALLEGRO_COLOR);
        const std::string get_name(void) const;
        const int get_width(void) const;
        const int get_height(void) const;
        ALLEGRO_BITMAP* get_background(void) const;

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
inline menu::menu(const std::string name, const int w, const int h, const ALLEGRO_COLOR color) {
    menu_name = name;
    background_bitmap = al_create_bitmap(w, h);
    al_set_target_bitmap(background_bitmap);
    al_clear_to_color(color);
}

//!  Copy constructor
/*!
*/
inline menu::menu(const menu& copy_menu) {
    menu_name = copy_menu.menu_name;
    background_bitmap = al_clone_bitmap(copy_menu.background_bitmap);
}

//!  Menu destructor
/*!
*/
inline menu::~menu() {
    al_destroy_bitmap(background_bitmap);
}

//!  Set menu background color
/*!
*/
inline void menu::set_bg_color(const ALLEGRO_COLOR color) {
    al_set_target_bitmap(background_bitmap);
    al_clear_to_color(color);
}

//!  Get menu name
/*!
*/
inline const std::string menu::get_name(void) const { return menu_name; }

//!  Get menu background bitmap width
/*!
*/
inline const int menu::get_width(void) const { return al_get_bitmap_width(background_bitmap); }

//!  Get menu background bitmap height
/*!
*/
inline const int menu::get_height(void) const { return al_get_bitmap_height(background_bitmap); }

//!  Get menu background bitmap
/*!
*/
inline ALLEGRO_BITMAP* menu::get_background(void) const { return background_bitmap; }

}  // end namespace mnu

}  // end namespace wte

#endif
