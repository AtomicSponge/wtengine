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

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

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
        inline menu() { background_bitmap = NULL; }
        menu(const std::string, const int, const int, const ALLEGRO_COLOR);
        menu(const menu&);
        inline ~menu() { al_destroy_bitmap(background_bitmap); }

        void set_bg_color(const ALLEGRO_COLOR);
        const bool add_item(const menu_item);

        inline const std::string get_id(void) const { return id; }
        inline const std::string get_title(void) const { return title; }
        inline const float get_padding(void) const { return padding; }
        inline const float get_width(void) const { return al_get_bitmap_width(background_bitmap); }
        inline const float get_height(void) const { return al_get_bitmap_height(background_bitmap); }
        inline ALLEGRO_BITMAP* get_background(void) const { return background_bitmap; }
        inline menu_items get_items(void) const { return items; }

        inline void set_title(const std::string t) { title = t; }

    private:
        menu_items items;

        std::string id;
        std::string title;

        float padding;

        ALLEGRO_BITMAP *background_bitmap;
};

//!  Menu constructor
/*!
*/
inline menu::menu(const std::string i, const int w, const int h, const ALLEGRO_COLOR color) {
    id = i;
    title = "";
    padding = 10;
    background_bitmap = al_create_bitmap(w, h);
    al_set_target_bitmap(background_bitmap);
    al_clear_to_color(color);
}

//!  Copy constructor
/*!
  Called to make a copy of the menu object
*/
inline menu::menu(const menu& copy_menu) {
    id = copy_menu.id;
    title = copy_menu.title;
    padding = copy_menu.padding;
    items = copy_menu.items;
    background_bitmap = al_clone_bitmap(copy_menu.background_bitmap);
}

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

}  // end namespace mnu

}  // end namespace wte

#endif
