/*!
 * WTEngine | File:  menus.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
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
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/renderer.hpp"
#include "wtengine/mnu/menu_item.hpp"
#include "wtengine/mnu/menu.hpp"
#include "wtengine/config.hpp"

namespace wte {
    class engine;
    class input;

    //! Menu iterator.
    typedef std::vector<mnu::menu_sptr>::iterator menu_iterator;
    //! Constant menu iterator.
    typedef std::vector<mnu::menu_sptr>::const_iterator menu_citerator;
}

namespace wte::mgr {

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
         * \brief Set a background to be used by the menus.
         * \param bmp Allegro bitmap asset.
         */
        static void set_background(wte_asset<al_bitmap> bmp);

        /*!
         * \brief Set menu padding.
         * \param p Padding value to set.
         */
        static void set_padding(const int& p);

        /*!
         * \brief Set a font to be used when rendering menus.
         * \param font Allegro font asset.
         * \param fcolor Allegro font color.
         */
        static void set_font(
            wte_asset<al_font> font,
            const ALLEGRO_COLOR& fcolor
        );

        /*!
         * \brief Set a bitmap to use as the menu cursor.
         * \param bmp Allegro bitmap asset.
         */
        static void set_cursor(wte_asset<al_bitmap> bmp);

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
        menus() = default;
        ~menus() = default;

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

        static ALLEGRO_TIMER* menu_timer;
        static ALLEGRO_EVENT_QUEUE* menu_event_queue;
        static ALLEGRO_COLOR menu_font_color;

        static wte_asset<al_bitmap> menu_buffer;
        static wte_asset<al_bitmap> menu_temp_bmp;
        static wte_asset<al_bitmap> menu_background;
        static wte_asset<al_font> menu_font;
        static wte_asset<al_bitmap> cursor_bitmap;

        static std::vector<mnu::menu_sptr> _menus;
        static std::stack<mnu::menu_csptr> opened_menus;
        static mnu::item_citerator menu_position;

        static int font_size;
        static int menu_padding;
        static int menu_width;
        static int menu_height;
        
        static bool select_menu_option;
        static bool is_button_left;
        
        static int64_t last_tick;
};

}  //  end namespace wte::mgr

#endif
