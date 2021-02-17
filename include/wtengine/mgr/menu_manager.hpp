/*!
 * WTEngine | File:  menu_manager.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
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

#include "wtenginenager.hpp"
#include "wtenginessage_manager.hpp"
#include "wtengine/mnu/menu_items.hpp"
#include "wtengine/_globals/engine_flags.hpp"
#include "wtengine/_globals/input_flags.hpp"
#include "wtengine/mnu/menu.hpp"
#include "wtengine/wte_global_defines.hpp"

namespace wte
{

//! Menu iterator.
typedef std::vector<mnu::menu_sptr>::iterator menu_iterator;
//! Constant menu iterator.
typedef std::vector<mnu::menu_sptr>::const_iterator menu_citerator;

namespace mgr
{

/*!
 * \class menu_manager
 * \brief Handles processing menus and rendering them.
 */
class menu_manager final : private manager<menu_manager> {
    public:
        /*!
         * \brief Menu manager constructor.
         * 
         * Generates the menu manager object.
         */
        menu_manager();

        /*!
         * \brief Menu manager destructor.
         * 
         * Cleans up by deleting the menu bitmaps and font.
         */
        ~menu_manager() {};

        /*!
         * \brief Ititialize menu manager.
         * 
         * Sets up internal menu objects.  Called during engine initialization.
         * Also creates the default main menu and in-game menu.
         */
        void initialize(void);

        /*!
         * \brief De-initialize the menu manager.
         * 
         * Destories the internal objects.
         */
        void de_init(void);

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
        void set_menu_size(const float& mw, const float& mh, const float& mp);

        /*!
         * \brief Reload the menu bitmap.
         * 
         * Called when the screen is updated.
         */
        void reload_menu_bitmap(void);

        /*!
         * \brief Set menu colors.
         * 
         * Call during menu creation.
         * 
         * \param fcolor Allegro color to use for font.
         * \param bgcolor Allegro color to use for background.
         */
        void set_menu_color(ALLEGRO_COLOR fcolor, ALLEGRO_COLOR bgcolor);

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
        static void set_font_file(const std::string& fname, const int& size, const int& flags);

        /*!
         * \brief Add a menu to the menu vector.
         * 
         * \param new_menu A menu object.
         * \return Returns false if a menu with a similar ID already exists.  Returns true on success.
         */
        const bool new_menu(const mnu::menu& new_menu);

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
        const mnu::menu_csptr get_menu(const std::string& name) const;

        /*!
         * \brief Set menu by name.
         * 
         * Finds a menu in the menu vector by name and returns it.
         * If not found, return a null pointer.
         * 
         * \param name Menu name.
         * \return Shared pointer to menu.
         */
        const mnu::menu_sptr set_menu(const std::string& name);

        /*!
         * \brief Reset menu manager.
         * 
         * Clear the stack of opened menus.
         */
        void reset(void);

        /*!
         * \brief Add a menu to the opened stack.
         * 
         * Takes a menu from the vector container and adds it to the top of the opened stack.
         * Also resets the menu position.
         * 
         * \param menu_id Menu to open.
         */
        void open_menu(const std::string& menu_id);

        /*!
         * \brief Close the current opened menu.
         * 
         * Remove the menu from the top of the stack.
         */
        void close_menu(void);

        /*!
         * \brief Run the menu manager.
         * 
         * Adds a menu to the stack if none are opened, then processes the menus.
         * 
         * \param messages Reference to the message manager.
         */
        void run(message_manager&);

        /*!
         * \brief Renders the active menu from the top of the stack.
         * 
         * This is called from within the renderer.  Drawing target must be reset after calling.
         * 
         * \return The rendered menu bitmap.
         */
        ALLEGRO_BITMAP& render_menu(void) const;

    private:
        mnu::menu_item_citerator menu_position;

        mutable ALLEGRO_BITMAP* menu_bitmap;
        ALLEGRO_BITMAP* cursor_bitmap;
        ALLEGRO_FONT* menu_font;
        ALLEGRO_COLOR menu_font_color;
        ALLEGRO_COLOR menu_bg_color;

        ALLEGRO_TIMER* menu_timer;
        ALLEGRO_EVENT_QUEUE* menu_event_queue;

        std::vector<mnu::menu_sptr> menus;
        std::stack<mnu::menu_csptr> opened_menus;

        float menu_width, menu_height, menu_padding;
        int font_size;

        bool is_button_left;

        inline static std::string menu_font_file = "";
        inline static int menu_font_size = 0;
        inline static int menu_font_flags = 0;
};

template <> bool menu_manager::manager<menu_manager>::initialized = false;

}  // end namespace mgr

}  // end namespace wte

#endif
