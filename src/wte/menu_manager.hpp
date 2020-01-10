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

#include "wte_globals.hpp"
#include "menu\menu.hpp"
#include "message_queue.hpp"

namespace wte
{

namespace mnu
{

typedef std::vector<menu>::const_iterator menu_citerator;

class menu_manager {
    public:
        menu_manager();
        menu_manager(ALLEGRO_FONT *);
        ~menu_manager();

        void new_menu(void);
        const bool add_item(const std::string, const menu_item);
        void reset(void);

        void run(msg::message_queue&);
        ALLEGRO_BITMAP* render_menu(void) const;

    private:
        menu_item_iterator menu_position;
        mutable ALLEGRO_BITMAP *menu_bitmap;
        ALLEGRO_FONT *menu_font;

        std::vector<menu> menus;
        std::stack<menu> opened_menus;

        void open_menu(const std::string);
        void close_menu(void);
        const menu get_menu(const std::string) const;
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
        menu temp_menu = menu("main_menu", 300, 200, WTE_COLOR_ORANGE);
        menus.push_back(temp_menu);
    }

    {
        //  Create the in-game menu
        menu temp_menu = menu("game_menu", 300, 200, WTE_COLOR_ORANGE);
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
  Finds a menu in the menu vector by name and returns a reference
  If not found, the first menu in the vector is returned
*/
inline const menu menu_manager::get_menu(const std::string name) const {
    for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
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
  Clear the stack of opened menus
*/
inline void menu_manager::reset(void) { opened_menus = {}; }

//!  Add a menu to the stack
/*!
  Performs a copy of a menu object and adds it to the top of the stack
  Also resets the menu position
*/
inline void menu_manager::open_menu(const std::string menu_name) {
    opened_menus.emplace(get_menu(menu_name));
    menu_position = opened_menus.top().get_items().begin();
}

//!  Close the current opened menu
/*!
  Remove the menu from the top of the stack
*/
inline void menu_manager::close_menu(void) { opened_menus.pop(); }

//!  Run the menu manager
/*!
  Adds a menu to the stack if none are opened, then processes the menus
*/
inline void menu_manager::run(msg::message_queue& messages) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack
        if(game_flag[GAME_STARTED]) open_menu("game_menu"); //  Add the in-game menu to the stack
        else open_menu("main_menu"); //  Add the main menu to the stack
    }

    //  Iterate through the menu items depending on key press
    if(key[KEY_UP] && menu_position != opened_menus.top().get_items().begin())
        menu_position--;
    if(key[KEY_DOWN] && menu_position != opened_menus.top().get_items().end())
        menu_position++;

    //...
}

//!  Render the active menu
/*!
  Renders the active menu from the top of the stack
*/
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) const {
    //  Destroy old bitmap if it exists
    al_destroy_bitmap(menu_bitmap);

    //  If the menu stack is empty then the run member hasn't been called yet
    //  Return a blank bitmap for now
    if(opened_menus.empty()) {
        menu_bitmap = al_create_bitmap(1, 1);
        return menu_bitmap;
    }

    //  Create a new menu bitmap and set drawing to it
    menu_bitmap = al_clone_bitmap(opened_menus.top().get_background());
    al_set_target_bitmap(menu_bitmap);

    //  Render menu text
    for(menu_item_citerator it = opened_menus.top().get_items().begin();
        it != opened_menus.top().get_items().end(); it++) {
        //
    }

    //  Return drawing to the screen
    al_set_target_backbuffer(al_get_current_display());
    return menu_bitmap;
}

}  // end namespace mnu

}  // end namespace wte

#endif
