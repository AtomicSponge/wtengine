/*!
 * \brief WTEngine | File:  menu_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Menu manager.
 * 
 */

#ifndef WTE_MGR_MENU_MANAGER_HPP
#define WTE_MGR_MENU_MANAGER_HPP

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "manager.hpp"
#include "../wte_global_defines.hpp"
#include "../engine_flags.hpp"
#include "../input_flags.hpp"
#include "../mnu/menu.hpp"
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
        //!  Menu manager constructor
        /*!
        Generates the menu manager object
        */
        inline menu_manager() : menu_width(340.0), menu_height(240.0), menu_padding(20.0), font_size(8) {
            menu_bitmap = NULL;
            cursor_bitmap = NULL;
            menu_font = NULL;

            menus.clear();
            opened_menus = {};
        }

        //!  Menu manager destructor
        /*!
        Cleans up by deleting the menu bitmaps and font
        */
        inline ~menu_manager() {
            menus.clear();
            opened_menus = {};

            al_destroy_bitmap(menu_bitmap);
            al_destroy_bitmap(cursor_bitmap);
            al_destroy_font(menu_font);
        }

        void initialize(ALLEGRO_FONT*, ALLEGRO_COLOR, ALLEGRO_COLOR);

        inline void set_width(const float mw) { menu_width = mw; };
        inline void set_height(const float mh) { menu_height = mh; };
        inline void set_padding(const float mp) { menu_padding = mp; };

        const bool new_menu(const mnu::menu);
        const menu_csptr get_menu(const std::string) const;
        const menu_sptr set_menu(const std::string);
        void reset(void);

        void open_menu(const std::string);
        void close_menu(void);

        void run(message_manager&);
        ALLEGRO_BITMAP* render_menu(void) const;

    private:
        mnu::menu_item_citerator menu_position;

        mutable ALLEGRO_BITMAP* menu_bitmap;
        ALLEGRO_BITMAP* cursor_bitmap;
        ALLEGRO_FONT* menu_font;
        ALLEGRO_COLOR menu_font_color;
        ALLEGRO_COLOR menu_bg_color;

        std::vector<menu_sptr> menus;
        std::stack<menu_csptr> opened_menus;

        float menu_width, menu_height, menu_padding;
        int font_size;
};

template <> inline bool menu_manager::manager<menu_manager>::initialized = false;

//!  Ititialize menu manager
/*!
  Pass an Allegro font for the menu manager to use
  Also create the default main menu and in-game menu
*/
inline void menu_manager::initialize(ALLEGRO_FONT* font, ALLEGRO_COLOR fcolor, ALLEGRO_COLOR bgcolor) {
    menu_font = font;
    menu_font_color = fcolor;
    menu_bg_color = bgcolor;

    //  Create default menus in seperate scopes
    {
        //  Create the main menu
        mnu::menu temp_menu = mnu::menu("main_menu", "Main Menu");
        if(!new_menu(temp_menu)) throw std::runtime_error("Unable to create main menu!");
    }

    {
        //  Create the in-game menu
        mnu::menu temp_menu = mnu::menu("game_menu", "Game Menu");
        if(!new_menu(temp_menu)) throw std::runtime_error("Unable to create game menu!");
    }

    font_size = al_get_font_line_height(menu_font);

    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    cursor_bitmap = al_create_bitmap(font_size, font_size);
    al_set_target_bitmap(cursor_bitmap);
    al_clear_to_color(menu_font_color);
    al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
}

//!  Add a menu to the menu vector
/*!
  Returns false if a menu with a similar ID already exists
  Returns true on success
*/
inline const bool menu_manager::new_menu(const mnu::menu new_menu) {
    for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
        if(new_menu.get_id() == (*it)->get_id()) return false;
    }
    menus.push_back(std::make_shared<mnu::menu>(new_menu));
    return true;
}

//!  Get menu by name
/*!
  Finds a menu in the menu vector by name and returns it
  If not found, try returning the main or game menu
  Return the first menu in the vector if no others found
*/
inline const menu_csptr menu_manager::get_menu(const std::string name) const {
    if(menus.empty()) throw std::runtime_error("No menus have been loaded!");

    for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }

    //  Menu not found, return main menu or game menu if the game is running
    if(engine_flags::is_set(GAME_STARTED)) {
        for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
            if("game_menu" == (*it)->get_id()) return *it;
        }
    } else {
        for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
            if("main_menu" == (*it)->get_id()) return *it;
        }
    }

    //  Menu still not found - just return the first one in the vector
    return *menus.begin();
}

//!  Set menu by name
/*!
  Finds a menu in the menu vector by name and returns it
  If not found, return a null pointer
*/
inline const menu_sptr menu_manager::set_menu(const std::string name) {
    if(menus.empty()) throw std::runtime_error("No menus have been loaded!");

    for(menu_iterator it = menus.begin(); it != menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }

    //  Menu not found - return null pointer
    return nullptr;
}

//!  Reset menu manager
/*!
  Clear the stack of opened menus
*/
inline void menu_manager::reset(void) {
    opened_menus = {};
    engine_flags::unset(GAME_MENU_OPENED);
}

//!  Add a menu to the stack
/*!
  Takes a menu from the vector container and adds it to the top of the opened stack
  Also resets the menu position
*/
inline void menu_manager::open_menu(const std::string menu_id) {
    opened_menus.push(get_menu(menu_id));
    engine_flags::set(GAME_MENU_OPENED);
    menu_position = opened_menus.top()->items_begin();
}

//!  Close the current opened menu
/*!
  Remove the menu from the top of the stack
*/
inline void menu_manager::close_menu(void) {
    opened_menus.pop();
    if(opened_menus.empty()) engine_flags::unset(GAME_MENU_OPENED);
    else menu_position = opened_menus.top()->items_begin();
}

//!  Run the menu manager
/*!
  Adds a menu to the stack if none are opened, then processes the menus
*/
inline void menu_manager::run(message_manager& messages) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack
        if(engine_flags::is_set(GAME_STARTED)) open_menu("game_menu"); //  Add the in-game menu to the stack
        else open_menu("main_menu"); //  Add the main menu to the stack
    }

    //  Iterate through the menu items depending on key press
    if(input_flags::is_set(INPUT_UP) && menu_position != opened_menus.top()->items_begin())
        menu_position--;
    if(input_flags::is_set(INPUT_DOWN) && menu_position != opened_menus.top()->items_end())
        menu_position++;
    //  The code above logically allows the index to go one beyond the vector.
    //  The code below fixes that :)
    if(menu_position == opened_menus.top()->items_end()) menu_position--;

    if(input_flags::is_set(INPUT_LEFT) && menu_position != opened_menus.top()->items_end())
        (*menu_position)->on_left();
    if(input_flags::is_set(INPUT_RIGHT) && menu_position != opened_menus.top()->items_end())
        (*menu_position)->on_right();

    if(input_flags::is_set(INPUT_MENU_SELECT) && menu_position != opened_menus.top()->items_end()) {
        message temp_msg = (*menu_position)->on_select();
        if(temp_msg.get_cmd() != "null") messages.add_message(temp_msg);
    }

    if(input_flags::is_set(INPUT_MENU_CLOSE)) reset();

    input_flags::unset_all();
}

//!  Render the active menu.
/*!
 * Renders the active menu from the top of the stack.
 */
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) const {
    //  Destroy old bitmap if it exists.
    al_destroy_bitmap(menu_bitmap);

    //  Create a new menu bitmap and set drawing to it.
    menu_bitmap = al_create_bitmap(menu_width, menu_height);
    al_set_target_bitmap(menu_bitmap);
    al_clear_to_color(menu_bg_color);

    //  If the menu stack is empty then the run member hasn't been called yet.
    //  Return a blank bitmap for now.
    if(opened_menus.empty()) return menu_bitmap;

    //  Render menu title.
    al_draw_text(menu_font, menu_font_color, menu_width / 2, menu_padding,
                 ALLEGRO_ALIGN_CENTER, opened_menus.top()->get_title().c_str());

    //  Render menu items.
    float cursor_pos = 10.0;
    float vpart = 0.0, hpart = 0.0, offset = 0.0;
    std::size_t vcounter = 0;

    offset = menu_padding + font_size + menu_padding;
    vpart = (menu_height - offset) / (opened_menus.top()->num_items() + 1);
    for(auto it = opened_menus.top()->items_cbegin(); it != opened_menus.top()->items_cend(); it++) {
        vcounter++;
        hpart = menu_width / ((*it)->get_text().size() + 1);
        for(std::size_t i = 0; i < (*it)->get_text().size(); i++)
            al_draw_text(menu_font, menu_font_color, hpart * (i + 1), (offset / 2) + (vpart * vcounter),
                         ALLEGRO_ALIGN_CENTER, (*it)->get_text()[i].c_str());
        if(it == menu_position) cursor_pos = (offset / 2) + (vpart * vcounter);
    }

    //  Render menu cursor.
    if(opened_menus.top()->num_items() != 0) al_draw_bitmap(cursor_bitmap, menu_padding, cursor_pos, 0);

    return menu_bitmap;
}

}  // end namespace mgr

}  // end namespace wte

#endif
