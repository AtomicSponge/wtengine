/*!
 * WTEngine | File:  menus.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_MENUS_HPP
#define WTE_MGR_MENUS_HPP

#include <string>
#include <vector>
#include <stack>
#include <memory>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/mgr/assets.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mnu/_menu_items.hpp"
#include "wtengine/mnu/menu.hpp"
#include "wtengine/config.hpp"

namespace wte
{

//! Menu iterator.
typedef std::vector<mnu::menu_sptr>::iterator menu_iterator;
//! Constant menu iterator.
typedef std::vector<mnu::menu_sptr>::const_iterator menu_citerator;

namespace mgr
{

/*!
 * \class menus
 * \brief Handles processing menus and rendering them.
 */
class menus final : private manager<menus> {
    friend class interface;
    friend class renderer;

    public:
        /*!
         * \brief Set menu size.
         * 
         * Call during menu creation.
         * If not called the default size of 500x400 padding 32 is used.
         * 
         * \param mw Menu width in pixels.
         * \param mh Menu height in pixels.
         * \param mp Menu padding in pixels.
         */
        inline static void set_menu_size(const float& mw, const float& mh, const float& mp) {
            menu_width = mw;
            menu_height = mh;
            menu_padding = mp;
            mgr::assets::secret_unload("wte_menu_bitmap");
            mgr::assets::secret_load<al_bitmap>("wte_menu_bitmap", menu_width, menu_height, true);
            al_set_target_bitmap(**mgr::assets::secret_get<al_bitmap>("wte_menu_bitmap"));
            al_clear_to_color(WTE_COLOR_BLACK);
        };

        /*!
         * \brief Set menu colors.
         * 
         * Call during menu creation.
         * 
         * \param fcolor Allegro color to use for font.
         * \param bgcolor Allegro color to use for background.
         */
        inline static void set_menu_color(const ALLEGRO_COLOR& fcolor, const ALLEGRO_COLOR& bgcolor) {
            menu_font_color = fcolor;
            menu_bg_color = bgcolor;

            al_set_target_bitmap(cursor_bitmap);
            al_clear_to_color(menu_font_color);
        };

        /*!
         * \brief Set the font to be used by the menus.
         * 
         * If not called, Allegro's default font will be used.
         * This should be called during engine initialization before the main object is created.
         * See the Allegro docs for more information on al_load_font and the parameters used.
         * 
         * \param fname Filename to use.
         * \param size Font size.
         * \param flags Font flags.
         */
        inline static void set_font_file(const std::string& fname, const int& size, const int& flags) {
            menu_font_file = fname;
            menu_font_size = size;
            menu_font_flags = flags;
        };

        /*!
         * \brief Add a menu to the menu vector.
         * 
         * \param new_menu A menu object.
         * \return Returns false if a menu with a similar ID already exists.  Returns true on success.
         */
        inline static const bool new_menu(const mnu::menu& new_menu) {
            for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
                if(new_menu.get_id() == (*it)->get_id()) return false;
            }
            _menus.push_back(std::make_shared<mnu::menu>(new_menu));
            return true;
        };

        /*!
         * \brief Get menu by name.
         * 
         * Finds a menu in the menu vector by name and returns it.
         * If not found, try returning the main or game menu.
         * Return the first menu in the vector if no others found.
         * 
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        inline static const mnu::menu_csptr get_menu(const std::string& name) {
            if(_menus.empty()) throw std::runtime_error("No menus have been loaded!");

            for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
                if(name == (*it)->get_id()) return *it;
            }

            //  Menu not found, return main menu or game menu if the game is running
            if(config::flags::game_started) {
                for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
                    if("game_menu" == (*it)->get_id()) return *it;
                }
            } else {
                for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
                    if("main_menu" == (*it)->get_id()) return *it;
                }
            }

            //  Menu still not found - just return the first one in the vector
            return *_menus.begin();
        };

        /*!
         * \brief Set menu by name.
         * 
         * Finds a menu in the menu vector by name and returns it.
         * If not found, return a null pointer.
         * 
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        inline static const mnu::menu_sptr set_menu(const std::string& name) {
            if(_menus.empty()) throw std::runtime_error("No menus have been loaded!");

            for(menu_iterator it = _menus.begin(); it != _menus.end(); it++) {
                if(name == (*it)->get_id()) return *it;
            }

            //  Menu not found - return null pointer
            return nullptr;
        };

        /*!
         * \brief Reset menu manager.
         * 
         * Clear the stack of opened menus.
         */
        static void reset(void);

        /*!
         * \brief Add a menu to the opened stack.
         * 
         * Takes a menu from the vector container and adds it to the top of the opened stack.
         * Also resets the menu position.
         * 
         * \param menu_id Menu to open.
         */
        static void open_menu(const std::string& menu_id);

        /*!
         * \brief Close the current opened menu.
         * 
         * Remove the menu from the top of the stack.
         */
        static void close_menu(void);

        /*!
         * \brief
         */
        static void menu_pos_up(void);

        /*!
         * \brief
         */
        static void menu_pos_down(void);

        /*!
         * \brief
         */
        static void menu_pos_start_left(void);

        /*!
         * \brief
         */
        static void menu_pos_stop_left(void);

        /*!
         * \brief
         */
        static void menu_pos_start_right(void);

        /*!
         * \brief
         */
        static void menu_pos_stop_right(void);

        /*!
         * \brief
         */
        static void menu_item_select(void);

    private:
        /*!
         * \brief Menu manager constructor.
         * 
         * Generates the menu manager object.
         */
        menus();

        /*!
         * \brief Menu manager destructor.
         * 
         * Cleans up by deleting the menu bitmaps and font.
         */
        ~menus();

        /*!
         * \brief Ititialize menu manager.
         * 
         * Sets up internal menu objects.  Called during engine initialization.
         * Also creates the default main menu and in-game menu.
         */
        static void initialize(void);

        /*!
         * \brief De-initialize the menu manager.
         * 
         * Destories the internal objects.
         */
        static void de_init(void);

        /*!
         * \brief Run the menu manager.
         * 
         * Adds a menu to the stack if none are opened, then processes the menus.
         */
        static void run(void);

        /*!
         * \brief Renders the active menu from the top of the stack.
         * 
         * This is called from within the renderer.  Drawing target must be reset after calling.
         * 
         * \return The rendered menu bitmap.
         */
        static ALLEGRO_BITMAP* render_menu(void);

        static mnu::menu_item_citerator menu_position;

        static ALLEGRO_BITMAP* cursor_bitmap;
        static ALLEGRO_FONT* menu_font;
        static ALLEGRO_COLOR menu_font_color;
        static ALLEGRO_COLOR menu_bg_color;

        static ALLEGRO_TIMER* menu_timer;
        static ALLEGRO_EVENT_QUEUE* menu_event_queue;

        static std::vector<mnu::menu_sptr> _menus;
        static std::stack<mnu::menu_csptr> opened_menus;

        static float menu_width, menu_height, menu_padding;
        static int font_size;

        static bool is_button_left;
        static bool toggle_menu_select;

        static std::string menu_font_file;
        static int menu_font_size;
        static int menu_font_flags;

        static bool do_render;
};

inline void menus::run(void) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack.
        if(config::flags::game_started) open_menu("game_menu");  //  Add the in-game menu to the stack.
        else open_menu("main_menu");  //  Add the main menu to the stack.
    }
    if(!al_get_timer_started(menu_timer)) al_resume_timer(menu_timer);  //  Make sure timer is running.

    ALLEGRO_EVENT event;
    const bool queue_not_empty = al_get_next_event(menu_event_queue, &event);
    if(queue_not_empty && event.type == ALLEGRO_EVENT_TIMER) {
        do_render = true;
        bool toggle_menu_item = false;

        /*if(al_get_timer_count(menu_timer) == 1) toggle_menu_item = true;
        if(al_get_timer_count(menu_timer) >= 20) {
            if(al_get_timer_count(menu_timer) >= 70) toggle_menu_item = true;
            else if(al_get_timer_count(menu_timer) % 5 == 0) toggle_menu_item = true;
        }*/

        if(toggle_menu_item) {
            if(is_button_left) (*menu_position)->on_left();
            else (*menu_position)->on_right();
        }
        //  End menu item processing.
    }
}

}  // end namespace mgr

}  // end namespace wte

#endif
