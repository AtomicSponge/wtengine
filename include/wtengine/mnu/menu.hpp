/*!
 * WTEngine | File:  menu.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_MNU_MENU_HPP
#define WTE_MNU_MENU_HPP

#include <string>
#include <vector>

#include "wtenginenu_item.hpp"

namespace wte
{

namespace mnu
{

//!  Container for storing menu items.
typedef std::vector<menu_item_sptr> menu_items;
//!  Constant iterator for addressing menu items.
typedef std::vector<menu_item_sptr>::const_iterator menu_item_citerator;

/*!
 * \class menu
 * \brief Class for defining a menu.
 */
class menu {
    public:
        /*!
         * \brief Menu constructor.
         * 
         * \param i Menu ID.
         * \param t Menu display title.
         */
        menu(const std::string i, const std::string t) :
        id(i), title(t) {
            items.clear();
        };

        /*!
         * \brief Menu destructor.
         */
        ~menu() {
            items.clear();
        };

        /*!
         * \brief Get menu ID.
         * 
         * \return The menu ID.
         */
        const std::string get_id(void) const {
            return id;
        };

        /*!
         * \brief Get menu display title.
         * 
         * \return The menu display title.
         */
        const std::string get_title(void) const {
            return title;
        };

        /*!
         * \brief Get number of menu items.
         * 
         * \return Count of items.
         */
        std::size_t num_items(void) const {
            return items.size();
        };

        /*!
         * \brief Get menu items start iterator.
         * 
         * \return Constant iterator to beginnig of menu items.
         */
        menu_item_citerator items_cbegin(void) const {
            return items.cbegin();
        };

        /*!
         * \brief Get menu items end iterator.
         * 
         * \return Constant iterator to end of menu items.
         */
        menu_item_citerator items_cend(void) const {
            return items.cend();
        };

        /*!
         * \brief Set the menu display title.
         * 
         * \param t New display title to set.
         */
        void set_title(const std::string& t) {
            title = t;
        };

        /*!
         * \brief Add a menu item to an existing menu.
         * 
         * \param item Shared pointer for the new item.
         */
        void add_item(const menu_item_sptr& item) {
            items.push_back(item);
        };

    private:
        menu_items items;

        std::string id;
        std::string title;
};

//! Shared pointer for addressing menu items.
typedef std::shared_ptr<mnu::menu> menu_sptr;
//! Constant shared pointer for addressing menu items.
typedef std::shared_ptr<const mnu::menu> menu_csptr;

}  // end namespace mnu

}  // end namespace wte

#endif
