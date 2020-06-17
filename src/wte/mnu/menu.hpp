/*!
 * \brief WTEngine | File:  menu.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Menu class.
 */

#ifndef WTE_MNU_MENU_HPP
#define WTE_MNU_MENU_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

//!  Container for storing menu items.
typedef std::vector<menu_item_sptr> menu_items;
//!  Constant iterator for addressing menu items.
typedef std::vector<menu_item_sptr>::const_iterator menu_item_citerator;

//!  Menu class.
/*!
 * Class for defining a menu.
 */
class menu {
    public:
        /*!
         * Menu constructor.
         * \param i Menu ID.
         * \param t Menu display title.
         * \return void
         */
        inline menu(const std::string i, const std::string t) :
        id(i), title(t) { items.clear(); };

        /*!
         * Menu destructor.
         * \param void
         * \return void
         */
        inline ~menu() { items.clear(); };

        /*!
         * Get menu ID.
         * \param void
         * \return The menu ID.
         */
        inline const std::string get_id(void) const { return id; };

        /*!
         * Get menu display title.
         * \param void
         * \return The menu display title.
         */
        inline const std::string get_title(void) const { return title; };

        /*!
         * Get number of menu items.
         * \param void
         * \return Count of items.
         */
        inline std::size_t num_items(void) const { return items.size(); };

        /*!
         * Get menu items start iterator.
         * \param void
         * \return Constant iterator to beginnig of menu items.
         */
        inline menu_item_citerator items_cbegin() const { return items.cbegin(); };

        /*!
         * Get menu items end iterator.
         * \param void
         * \return Constant iterator to end of menu items.
         */
        inline menu_item_citerator items_cend() const { return items.cend(); };

        /*!
         * Set the menu display title.
         * \param t New display title to set.
         * \return void
         */
        inline void set_title(const std::string t) { title = t; };

        /*!
         * Add a menu item to an existing menu
         * \param item Shared pointer for the new item.
         * \return void
         */
        inline void add_item(const menu_item_sptr item) { items.push_back(item); };

    private:
        menu_items items;

        std::string id;
        std::string title;
};

}  // end namespace mnu

}  // end namespace wte

#endif
