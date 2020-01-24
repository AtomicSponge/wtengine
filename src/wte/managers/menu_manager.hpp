/*
  WTEngine
  By:  Matthew Evans
  File:  menu_manager.hpp

  See LICENSE.txt for copyright information

  Menu manager
*/

#ifndef WTE_MGR_MENU_MANAGER_HPP
#define WTE_MGR_MENU_MANAGER_HPP

#include <string>
#include <vector>
#include <stack>
#include <memory>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "manager.hpp"
#include "..\wte_globals.hpp"
#include "..\sys_flags.hpp"
#include "..\key_flags.hpp"
#include "..\menu\menu.hpp"
#include "message_manager.hpp"

namespace wte
{

typedef std::shared_ptr<mnu::menu> menu_sptr;
typedef std::shared_ptr<const mnu::menu> menu_csptr;

typedef std::vector<menu_sptr>::iterator menu_iterator;
typedef std::vector<menu_sptr>::const_iterator menu_citerator;

namespace mgr
{

class menu_manager final : public manager<menu_manager> {
    public:
        menu_manager();
        ~menu_manager();

        void initialize(ALLEGRO_FONT *, ALLEGRO_COLOR);

        void new_menu(const mnu::menu);
        const menu_csptr get_menu(const std::string) const;
        const menu_sptr set_menu(const std::string);
        void reset(void);

        void open_menu(const std::string);
        void close_menu(void);

        void run(message_manager&);
        ALLEGRO_BITMAP* render_menu(void) const;

    private:
        mnu::menu_item_citerator menu_position;
        mnu::option_citerator option_selection;

        mutable ALLEGRO_BITMAP *menu_bitmap;
        ALLEGRO_BITMAP *menu_cursor;
        ALLEGRO_FONT *menu_font;
        ALLEGRO_COLOR menu_font_color;

        std::vector<menu_sptr> menus;
        std::stack<menu_csptr> opened_menus;
};

template <> inline bool menu_manager::manager<menu_manager>::initialized = false;

//!  Menu manager constructor
/*!
  Generates the menu manager object
*/
inline menu_manager::menu_manager() {
    menu_bitmap = NULL;
    menu_cursor = NULL;
    menu_font = NULL;

    menus.clear();
    opened_menus = {};
}

//!  Menu manager destructor
/*!
  Cleans up by deleting the menu bitmap and font
*/
inline menu_manager::~menu_manager() {
    menus.clear();
    opened_menus = {};

    al_destroy_bitmap(menu_bitmap);
    al_destroy_bitmap(menu_cursor);
    al_destroy_font(menu_font);
}

//!  Ititialize menu manager
/*!
  Pass an Allegro font for the menu manager to use
  Also create the default main menu and in-game menu
*/
inline void menu_manager::initialize(ALLEGRO_FONT *font, ALLEGRO_COLOR color) {
    menu_font = font;
    menu_font_color = color;

    //  Create default menus in seperate scopes
    {
        //  Create the main menu
        mnu::menu temp_menu = mnu::menu("main_menu", 300, 200, WTE_COLOR_DARKPURPLE);
        new_menu(temp_menu);
    }

    {
        //  Create the in-game menu
        mnu::menu temp_menu = mnu::menu("game_menu", 300, 200, WTE_COLOR_DARKPURPLE);
        new_menu(temp_menu);
    }

    menu_cursor = al_create_bitmap(8, 8);
    al_set_target_bitmap(menu_cursor);
    al_clear_to_color(color);
    al_set_target_backbuffer(al_get_current_display());
}

//!  Add a menu to the menu vector
/*!
*/
inline void menu_manager::new_menu(const mnu::menu new_menu) {
    menus.push_back(std::make_shared<mnu::menu>(new_menu));
}

//!  Get menu by name
/*!
  Finds a menu in the menu vector by name and returns it
  If not found, the first menu in the vector is returned
*/
inline const menu_csptr menu_manager::get_menu(const std::string name) const {
    for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }
    //  Menu not found - just return the first one in the vector
    return *menus.begin();
}

//!  Set menu by name
/*!
  Finds a menu in the menu vector by name and returns it
  If not found, the first menu in the vector is returned
*/
inline const menu_sptr menu_manager::set_menu(const std::string name) {
    for(menu_iterator it = menus.begin(); it != menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }
    //  Menu not found - just return the first one in the vector
    return *menus.begin();
}

//!  Reset menu manager
/*!
  Clear the stack of opened menus
*/
inline void menu_manager::reset(void) {
    opened_menus = {};
    sys_flags::unset(GAME_MENU_OPENED);
}

//!  Add a menu to the stack
/*!
  Takes a menu from the vector container and adds it to the top of the opened stack
  Also resets the menu position
*/
inline void menu_manager::open_menu(const std::string menu_id) {
    opened_menus.push(get_menu(menu_id));
    sys_flags::set(GAME_MENU_OPENED);
    menu_position = opened_menus.top()->get_items().begin();
}

//!  Close the current opened menu
/*!
  Remove the menu from the top of the stack
*/
inline void menu_manager::close_menu(void) {
    opened_menus.pop();
    if(opened_menus.empty()) sys_flags::unset(GAME_MENU_OPENED);
}

//!  Run the menu manager
/*!
  Adds a menu to the stack if none are opened, then processes the menus
*/
inline void menu_manager::run(message_manager& messages) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack
        if(sys_flags::is_set(GAME_STARTED)) open_menu("game_menu"); //  Add the in-game menu to the stack
        else open_menu("main_menu"); //  Add the main menu to the stack
    }

    //  Iterate through the menu items depending on key press
    if(key_flags::is_set(KEY_UP) && menu_position != opened_menus.top()->get_items().begin()) {
        menu_position--;
    }
    if(key_flags::is_set(KEY_DOWN) && menu_position != opened_menus.top()->get_items().end()) {
        menu_position++;
    }

    //if(key_flags::is_set(KEY_LEFT) && menu_position != opened_menus.top()->get_items().begin())
    //    option_selection--;
    //if(key_flags::is_set(KEY_RIGHT) && menu_position != opened_menus.top()->get_items().end())
    //    option_selection++;

    //...
}

//!  Render the active menu
/*!
  Renders the active menu from the top of the stack
*/
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) const {
    bool has_title = false;
    //  menu_item_citerator -> iterate items
    //  option_citerator -> iterate options of an item

    //  Destroy old bitmap if it exists
    al_destroy_bitmap(menu_bitmap);

    //  If the menu stack is empty then the run member hasn't been called yet
    //  Return a blank bitmap for now
    if(opened_menus.empty()) {
        menu_bitmap = al_create_bitmap(1, 1);
        return menu_bitmap;
    }

    //  Create a new menu bitmap and set drawing to it
    menu_bitmap = al_clone_bitmap(opened_menus.top()->get_background());
    al_set_target_bitmap(menu_bitmap);

    /*
      Render menu text
    */
    //  Render menu title if one is set
    if(opened_menus.top()->get_title() != "") {
        al_draw_text(menu_font, menu_font_color,
                     opened_menus.top()->get_width() / 2, opened_menus.top()->get_padding(),
                     ALLEGRO_ALIGN_CENTER, opened_menus.top()->get_title().c_str());
        has_title = true;
    }

    //  Render menu items
    for(mnu::menu_item_citerator it = opened_menus.top()->get_items().begin();
        it != opened_menus.top()->get_items().end(); it++) {
        //it->get_label();
        //al_draw_text(menu_font, menu_font_color, x, y, ALLEGRO_ALIGN_CENTER, str.c_str())
    }

    //  Return drawing to the screen
    al_set_target_backbuffer(al_get_current_display());
    return menu_bitmap;
}

}  // end namespace mgr

}  // end namespace wte

#endif
