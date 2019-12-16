/*
  Menu manager
  By:  Matthew Evans
  File:  menu_manager.hpp

  See LICENSE.txt for copyright information

  Menu manager
*/

#ifndef WTE_MENU_MENU_MANAGER_HPP
#define WTE_MENU_MENU_MANAGER_HPP

#include <string>
#include <vector>
#include <stack>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "menu\menu.hpp"

namespace wte
{

namespace mnu
{

typedef unsigned int menu_id;

class menu_manager {
    public:
        menu_manager();
        ~menu_manager();
        const menu_id new_menu(void);
        const bool add_item(const menu_id, const menu_item);
        void run(void);
        ALLEGRO_BITMAP* render_menu(void);

    private:
        unsigned int menu_position;
        ALLEGRO_BITMAP *menu_bitmap;

        std::vector<menu> menus;
        std::stack<menu> opened_menus;
};

//!  Menu manager constructor
/*!
  Initializes the menu manager and sets the menu bitmap to null
*/
inline menu_manager::menu_manager() {
    menu_bitmap = NULL;
}

//!  Menu manager destructor
/*!
  Cleans up by deleting the menu bitmap
*/
inline menu_manager::~menu_manager() {
    al_destroy_bitmap(menu_bitmap);
}

//!
/*!
*/
inline const menu_id menu_manager::new_menu(void) {
    menu_id next_id;
    return next_id;
}

//!
/*!
*/
inline const bool menu_manager::add_item(const menu_id id, const menu_item item) {
    return true;
}

//!
/*!
*/
inline void menu_manager::run(void) {
    //
}

//!  Render the active menu
/*!
  Pulls the active menu off the top of the stack and renders
*/
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) {
    //  Destroy old bitmap if it exists
    al_destroy_bitmap(menu_bitmap);

    //  Create a new menu bitmap and set drawing to it
    menu_bitmap = al_create_bitmap(300, 200);
    al_set_target_bitmap(menu_bitmap);

    //  Test code to return a bitmap
    al_clear_to_color(al_map_rgb(255, 165, 0));

    //  Return drawing to the screen
    al_set_target_backbuffer(al_get_current_display());
    return menu_bitmap;
}

}  // end namespace mnu

}  // end namespace wte

#endif
