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
#include "wte_globals.hpp"

namespace wte
{

namespace mnu
{

typedef std::vector<menu>::iterator menu_iterator;

class menu_manager {
    public:
        menu_manager();
        menu_manager(ALLEGRO_FONT *);
        ~menu_manager();

        void new_menu(void);
        const menu get_menu(const std::string);
        const bool add_item(const std::string, const menu_item);
        void reset(void);
        void run(void);
        ALLEGRO_BITMAP* render_menu(void);

    private:
        unsigned int menu_position;
        ALLEGRO_BITMAP *menu_bitmap;
        ALLEGRO_FONT *menu_font;

        std::vector<menu> menus;
        std::stack<menu> opened_menus;
};

//!  Menu manager default constructor
/*!
*/
inline menu_manager::menu_manager() {
    menu_bitmap = NULL;
    menu_font = NULL;
}

//!  Menu manager constructor
/*!
  Initializes the menu manager and sets the menu bitmap to null
  Also pre-generates the main menu and in-game menu
  Gets passed an Allegro font for the menus to use
*/
inline menu_manager::menu_manager(ALLEGRO_FONT *font) {
    menu_bitmap = NULL;
    menu_font = font;

    //  Create default menus in seperate scopes
    {
        //  Create the main menu
        menu temp_menu = menu("main_menu", 300, 200);
        menus.push_back(temp_menu);
    }

    {
        //  Create the in-game menu
        menu temp_menu = menu("game_menu", 300, 200);
        menus.push_back(temp_menu);
    }
}

//!  Menu manager destructor
/*!
  Cleans up by deleting the menu bitmap and font
*/
inline menu_manager::~menu_manager() {
    al_destroy_bitmap(menu_bitmap);
    //al_destroy_font(menu_font);
}

//!  Add a menu to the menu list
/*!
*/
inline void menu_manager::new_menu(void) {
    //
}

//!  Get menu by name
/*!
*/
inline const menu menu_manager::get_menu(const std::string name) {
    for(menu_iterator it = menus.begin(); it != menus.end(); it++) {
        if(name == it->get_name()) return *it;
    }
    //  Menu not found - just return the first one in the list
    return *menus.begin();
}

//!  Add a menu item to an existing menu
/*!
*/
inline const bool menu_manager::add_item(const std::string menu_name, const menu_item item) {
    return true;
}

//!  Reset menu manager
/*!
  Clear the stack of opened menus and reset the index
*/
inline void menu_manager::reset(void) {
    menu_position = 0;
    opened_menus = {};
}

//!  Run the menu manager
/*!
  Adds a menu to the stack if none are opened, then processes the menus
*/
inline void menu_manager::run(void) {
    //  No menus currently opened, add one to the stack
    if(opened_menus.empty()) {
        if(game_flag[GAME_STARTED]) {
            //  Add the in-game menu to the stack
            opened_menus.emplace(get_menu("game_menu"));
        } else {
            //  Add the main menu to the stack
            opened_menus.emplace(get_menu("main_menu"));
        }
    }
}

//!  Render the active menu
/*!
  Renders the active menu from the top of the stack
*/
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) {
    //  Destroy old bitmap if it exists
    al_destroy_bitmap(menu_bitmap);

    //  If the menu stack is empty, call the run member
    if(opened_menus.empty()) run();

    //  Create a new menu bitmap and set drawing to it
    menu_bitmap = al_clone_bitmap(opened_menus.top().get_background());
    al_set_target_bitmap(menu_bitmap);

    //  Draw text...

    //  Return drawing to the screen
    al_set_target_backbuffer(al_get_current_display());
    return menu_bitmap;
}

}  // end namespace mnu

}  // end namespace wte

#endif
