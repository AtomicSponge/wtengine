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
        inline menu() {};
        menu(std::string, float, float);
        ~menu();

        const float get_width(void) const;
        const float get_height(void) const;

    private:
        std::string menu_name;
        float width, height;
        std::vector<menu_item> items;
        ALLEGRO_BITMAP *background_bitmap;
};

//!
/*!
*/
inline menu::menu(std::string name, float w, float h) {
    menu_name = name;
    width = w;
    height = h;

    background_bitmap = al_create_bitmap(w, h);
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
inline const float menu::get_width(void) const { return width; }

//!
/*!
*/
inline const float menu::get_height(void) const { return height; }

}  // end namespace mnu

}  // end namespace wte

#endif
