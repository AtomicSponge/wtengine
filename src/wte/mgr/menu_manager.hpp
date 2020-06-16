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
#include "message_manager.hpp"
#include "../mnu/menu_items.hpp"
#include "../wte_global_defines.hpp"
#include "../engine_flags.hpp"
#include "../input_flags.hpp"
#include "../mnu/menu.hpp"

namespace wte
{

//! Shared pointer for addressing menu items.
typedef std::shared_ptr<mnu::menu> menu_sptr;
//! Constant shared pointer for addressing menu items.
typedef std::shared_ptr<const mnu::menu> menu_csptr;

//! Menu iterator.
typedef std::vector<menu_sptr>::iterator menu_iterator;
//! Constant menu iterator.
typedef std::vector<menu_sptr>::const_iterator menu_citerator;

namespace mgr
{

class menu_manager final : public manager<menu_manager> {
    public:
        /*!
         * \brief Menu manager constructor
         * Generates the menu manager object
         */
        inline menu_manager() : font_size(8) {
            menus.clear();
            opened_menus = {};
        };

        /*!
         * \brief Menu manager destructor
         * Cleans up by deleting the menu bitmaps and font
         */
        inline ~menu_manager() {
            menus.clear();
            opened_menus = {};
        };

        /*!
         * \brief Ititialize menu manager.
         * Sets up internal menu objects.
         * Also create the default main menu and in-game menu.
         * \param fcolor Allegro color to use for font.
         * \param bgcolor Allegro color to use for background.
         * \return void
         */
        inline void initialize(ALLEGRO_COLOR fcolor, ALLEGRO_COLOR bgcolor) {
            if(menu_font_file.empty()) menu_font = al_create_builtin_font();
            else {
                menu_font = al_load_font(menu_font_file.c_str(), menu_font_size, menu_font_flags);
                if(!menu_font) menu_font = al_create_builtin_font();
            }
            if(!menu_font) throw std::runtime_error("Unable to set font for menus!");

            menu_font_color = fcolor;
            menu_bg_color = bgcolor;

            //  Create the main menu
            mnu::menu temp_main_menu = mnu::menu("main_menu", "Main Menu");
            if(!new_menu(temp_main_menu)) throw std::runtime_error("Unable to create main menu!");

            //  Create the in-game menu
            mnu::menu temp_game_menu = mnu::menu("game_menu", "Game Menu");
            if(!new_menu(temp_game_menu)) throw std::runtime_error("Unable to create game menu!");

            font_size = al_get_font_line_height(menu_font);

            cursor_bitmap = al_create_bitmap(font_size, font_size);
            al_set_target_bitmap(cursor_bitmap);
            al_clear_to_color(menu_font_color);

            menu_bitmap = al_create_bitmap(menu_width, menu_height);
        };

        /*!
         * \brief De-initialize the menu manager.
         * Destories the internal objects.
         * \param void
         * \return void
         */
        inline void de_init(void) {
            al_destroy_bitmap(menu_bitmap);
            al_destroy_bitmap(cursor_bitmap);
            al_destroy_font(menu_font);
        };

        /*!
         * \brief Set menu size.
         * This should be called during engine initialization before the main object is created.
         * If not called the default size of 500x400 padding 32 is used.
         * \param mw Menu width in pixels.
         * \param mh Menu height in pixels.
         * \param mp Menu padding in pixels.
         * \return void
         */
        inline static void set_menu_size(const float mw, const float mh, const float mp) {
            menu_width = mw;
            menu_height = mh;
            menu_padding = mp;
        };

        /*!
         * Add a menu to the menu vector.
         * \param new_menu A menu object.
         * \return Returns false if a menu with a similar ID already exists.  Returns true on success.
         */
        inline const bool new_menu(const mnu::menu new_menu) {
            for(menu_citerator it = menus.begin(); it != menus.end(); it++) {
                if(new_menu.get_id() == (*it)->get_id()) return false;
            }
            menus.push_back(std::make_shared<mnu::menu>(new_menu));
            return true;
        };

        /*!
         * \brief Get menu by name.
         * Finds a menu in the menu vector by name and returns it.
         * If not found, try returning the main or game menu.
         * Return the first menu in the vector if no others found.
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        inline const menu_csptr get_menu(const std::string name) const {
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
        };

        /*!
         * \brief Set menu by name.
         * Finds a menu in the menu vector by name and returns it.
         * If not found, return a null pointer.
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        inline const menu_sptr set_menu(const std::string name) {
            if(menus.empty()) throw std::runtime_error("No menus have been loaded!");

            for(menu_iterator it = menus.begin(); it != menus.end(); it++) {
                if(name == (*it)->get_id()) return *it;
            }

            //  Menu not found - return null pointer
            return nullptr;
        };

        /*!
         * \brief Reset menu manager.
         * Clear the stack of opened menus.
         * \param void
         * \return void
         */
        inline void reset(void) {
            opened_menus = {};
            engine_flags::unset(GAME_MENU_OPENED);
        };

        /*!
         * \brief Add a menu to the opened stack.
         * Takes a menu from the vector container and adds it to the top of the opened stack.
         * Also resets the menu position.
         * \param menu_id Menu to open.
         * \return void
         */
        inline void open_menu(const std::string menu_id) {
            opened_menus.push(get_menu(menu_id));
            engine_flags::set(GAME_MENU_OPENED);
            menu_position = opened_menus.top()->items_begin();

            //  Set default values for any menu settings objects.
            for(auto it = opened_menus.top()->items_begin(); it != opened_menus.top()->items_end(); it++) {
                (*it)->set_default();
            }
        };

        /*!
         * \brief Close the current opened menu.
         * Remove the menu from the top of the stack.
         * \param void
         * \return void
         */
        inline void close_menu(void) {
            opened_menus.pop();
            if(opened_menus.empty()) engine_flags::unset(GAME_MENU_OPENED);
            else menu_position = opened_menus.top()->items_begin();
        };

        inline static void set_font_file(const std::string fname, const int size, const int flags) {
            menu_font_file = fname;
            menu_font_size = size;
            menu_font_flags = flags;
        };

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

        int font_size;

        inline static float menu_width = 500, menu_height = 400, menu_padding = 32;
        inline static std::string menu_font_file = "";
        inline static int menu_font_size = 0;
        inline static int menu_font_flags = 0;
};

template <> inline bool menu_manager::manager<menu_manager>::initialized = false;

/*!
 * \brief Run the menu manager.
 * Adds a menu to the stack if none are opened, then processes the menus.
 * \param messages Reference to message manager.
 * \return void
 */
inline void menu_manager::run(message_manager& messages) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack
        if(engine_flags::is_set(GAME_STARTED)) open_menu("game_menu"); //  Add the in-game menu to the stack.
        else open_menu("main_menu"); //  Add the main menu to the stack.
    }

    //  Iterate through the menu items depending on key press.
    if(input_flags::is_set(INPUT_UP) && menu_position != opened_menus.top()->items_begin()) {
        menu_position--;
    }
    if(input_flags::is_set(INPUT_DOWN) && menu_position != opened_menus.top()->items_end()) {
        menu_position++;
        if(menu_position == opened_menus.top()->items_end()) menu_position--;
    }

    if(input_flags::is_set(INPUT_LEFT) && menu_position != opened_menus.top()->items_end())
        (*menu_position)->on_left();
    if(input_flags::is_set(INPUT_RIGHT) && menu_position != opened_menus.top()->items_end())
        (*menu_position)->on_right();

    if(input_flags::is_set(INPUT_MENU_SELECT) && menu_position != opened_menus.top()->items_end()) {
        message temp_msg = (*menu_position)->on_select();
        //  Check if the message is for the menu system and process.
        if(temp_msg.get_sys() == "menu") {
            if(temp_msg.get_cmd() == "apply") {
                //  Do apply.  Go through menu items, find all actionable menu objects and process.
                std::string eng_settings_string = "";
                std::string game_settings_string = "";

                for(auto it = opened_menus.top()->items_cbegin(); it != opened_menus.top()->items_cend(); it++) {
                    //  Process menu setting objects.
                    if(std::dynamic_pointer_cast<mnu::menu_item_setting>(*it)) {
                        //  See if the setting is a game or engine setting, add to correct string.
                        if(std::static_pointer_cast<mnu::menu_item_setting>(*it)->is_engine_setting()) {
                            if(!eng_settings_string.empty()) eng_settings_string += ";";
                            eng_settings_string += std::static_pointer_cast<mnu::menu_item_setting>(*it)->get_setting();
                        } else {
                            if(!game_settings_string.empty()) game_settings_string += ";";
                            game_settings_string += std::static_pointer_cast<mnu::menu_item_setting>(*it)->get_setting();
                        }
                    }

                    //  Process menu toggle objects.
                    if(std::dynamic_pointer_cast<mnu::menu_item_toggle>(*it)) {
                        messages.add_message(message("system",
                                             std::static_pointer_cast<mnu::menu_item_toggle>(*it)->get_active_cmd(),
                                             std::static_pointer_cast<mnu::menu_item_toggle>(*it)->get_active_args()));
                    }
                }

                //  Send messages in reverse order for system to process correctly.
                messages.add_message(message("system", "alert", "Settings applied."));

                //  Apply engine settings if any.
                if(!eng_settings_string.empty()) {
                    //messages.add_message(message("system", "reconf_display", ""));
                    messages.add_message(message("system", "set_engcfg", eng_settings_string));
                }

                //  Apply game settings if any.
                if(!game_settings_string.empty()) {
                    messages.add_message(message("system", "set_gamecfg", game_settings_string));
                }
            }
            if(temp_msg.get_cmd() == "cancel") {
                //  Do cancel.  Go through menu items, reset menu settings objects to their defaults.
                for(auto it = opened_menus.top()->items_begin(); it != opened_menus.top()->items_end(); it++) {
                    (*it)->reset_to_default();
                }
            }
        }  //  End menu messages.
        //  If not for the menu system, add to the message queue.
        else if(temp_msg.get_cmd() != "null") messages.add_message(temp_msg);
    }

    if(input_flags::is_set(INPUT_MENU_CLOSE)) reset();

    input_flags::unset_all();
}

/*!
 * \brief Renders the active menu from the top of the stack.
 * This is called from within the renderer.  Drawing target must be reset after calling.
 * \param void
 * \return The menu bitmap.
 */
inline ALLEGRO_BITMAP* menu_manager::render_menu(void) const {
    //  Set drawing to the menu bitmap.
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
