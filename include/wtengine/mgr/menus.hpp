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
#include "wtengine/mgr/renderer.hpp"
#include "wtengine/mnu/_menu_items.hpp"
#include "wtengine/mnu/menu.hpp"
#include "wtengine/config.hpp"

namespace wte
{

class engine;
class input;

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
    friend class wte::engine;
    friend class wte::input;
    friend class renderer;

    public:
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
         * \param new_menu A menu object.
         * \return Returns false if a menu with a similar ID already exists.  Returns true on success.
         */
        static const bool new_menu(const mnu::menu& new_menu);

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
        static const mnu::menu_csptr get_menu(const std::string& name);

        /*!
         * \brief Set menu by name.
         * 
         * Finds a menu in the menu vector by name and returns it.
         * If not found, return a null pointer.
         * 
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        static const mnu::menu_sptr set_menu(const std::string& name);

    private:
        menus();
        ~menus();

        /*
         * Sets up internal menu objects.  Called during engine initialization.
         * Also creates the default main menu and in-game menu.
         */
        static void initialize(void);

        /*
         * Destories the internal objects.
         */
        static void de_init(void);

        /*
         * Pre-renders game menus.
         */
        static void generate_menus(void);

        /*
         * Adds a menu to the stack if none are opened, then processes the menus.
         */
        static void run(void);

        /*
         * Renders the active menu from the top of the stack. 
         * This is called from within the renderer.
         */
        static ALLEGRO_BITMAP* render_menu(void);

        /*
         * Clear the stack of opened menus.
         */
        static void reset(void);

        /*
         * Add a menu to the opened stack.
         */
        static void open_menu(const std::string& menu_id);

        /*
         * Close the current opened menu.
         */
        static void close_menu(void);

        /* *************************************** */
        /* *** START MENU NAVIGATION FUNCTIONS *** */
        /* *************************************** */
        static void menu_pos_up(void);
        static void menu_pos_down(void);
        static void menu_pos_start_left(void);
        static void menu_pos_stop_left(void);
        static void menu_pos_start_right(void);
        static void menu_pos_stop_right(void);
        static void menu_item_select(void);
        /* *************************************** */
        /* ***  END MENU NAVIGATION FUNCTIONS  *** */
        /* *************************************** */

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

        static bool select_menu_option;
        static bool is_button_left;

        static std::string menu_font_file;
        static int menu_font_size;
        static int menu_font_flags;

        static int64_t last_tick;
        static bool do_render;
};

}  // end namespace mgr

}  // end namespace wte

#endif
