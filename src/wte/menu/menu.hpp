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

typedef std::vector<menu_item> menu_items;
typedef std::vector<menu_item>::iterator menu_item_iterator;
typedef std::vector<menu_item>::const_iterator menu_item_citerator;

class menu {
    public:
        menu();
        menu(const std::string, const int, const int, const ALLEGRO_COLOR);
        menu(const menu&);
        ~menu();

        void set_bg_color(const ALLEGRO_COLOR);
        const bool add_item(const menu_item);
        const std::string get_name(void) const;
        const int get_width(void) const;
        const int get_height(void) const;
        ALLEGRO_BITMAP* get_background(void) const;
        menu_items get_items(void) const;

    private:
        std::string menu_name;
        menu_items items;
        ALLEGRO_BITMAP *background_bitmap;
};

//!  Default constructor
/*!
*/
inline menu::menu() { background_bitmap = NULL; }

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
  Called to make a copy of the menu object
*/
inline menu::menu(const menu& copy_menu) {
    menu_name = copy_menu.menu_name;
    items = copy_menu.items;
    background_bitmap = al_clone_bitmap(copy_menu.background_bitmap);
}

//!  Menu destructor
/*!
  Deletes the background bitmap 
*/
inline menu::~menu() { al_destroy_bitmap(background_bitmap); }

//!  Set menu background color
/*!
*/
inline void menu::set_bg_color(const ALLEGRO_COLOR color) {
    al_set_target_bitmap(background_bitmap);
    al_clear_to_color(color);
}

//!  Add a menu item to an existing menu
/*!
*/
inline const bool menu::add_item(const menu_item item) {
    items.push_back(item);
    return true;
}

//!  Get menu name
/*!
  Returns the name of the menu
*/
inline const std::string menu::get_name(void) const { return menu_name; }

//!  Get menu background bitmap width
/*!
  Returns the width of the background bitmap
*/
inline const int menu::get_width(void) const { return al_get_bitmap_width(background_bitmap); }

//!  Get menu background bitmap height
/*!
  Returns the height of the background bitmap
*/
inline const int menu::get_height(void) const { return al_get_bitmap_height(background_bitmap); }

//!  Get menu background bitmap
/*!
  Returns a pointer to the background bitmap
*/
inline ALLEGRO_BITMAP* menu::get_background(void) const { return background_bitmap; }

//!  Get menu items
/*!
  Returns the menu item vector
*/
inline menu_items menu::get_items(void) const { return items; }

}  // end namespace mnu

}  // end namespace wte

#endif
