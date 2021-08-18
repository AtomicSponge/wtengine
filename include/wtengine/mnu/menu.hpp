/*!
 * WTEngine | File:  menu.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_MENU_HPP
#define WTE_MNU_MENU_HPP

#include <string>
#include <vector>
#include <memory>

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/mgr/assets.hpp"
#include "wtengine/mnu/menu_item.hpp"

namespace wte {

namespace mgr {

class menus;

}

namespace mnu {

//!  Container for storing menu items.
typedef std::vector<menu_item_sptr> menu_items;
//!  Constant iterator for addressing menu items.
typedef std::vector<menu_item_sptr>::const_iterator menu_item_citerator;

/*!
 * \class menu
 * \brief Class for defining a menu.
 */
class menu {
    friend class wte::mgr::menus;

    public:
        /*!
         * \brief Menu constructor.
         * \param i Menu ID.
         * \param t Menu display title.
         */
        menu(
            const std::string i,
            const std::string t
        );

        ~menu();

        /*!
         * \brief Get menu ID.
         * \return The menu ID.
         */
        const std::string get_id(void) const;

        /*!
         * \brief Get menu display title.
         * \return The menu display title.
         */
        const std::string get_title(void) const;

        /*!
         * \brief Get number of menu items.
         * \return Count of items.
         */
        const std::size_t num_items(void) const;

        /*!
         * \brief Set the menu display title.
         * \param t New display title to set.
         */
        void set_title(const std::string& t);

        /*!
         * \brief Add a menu item to an existing menu.
         * \tparam T Menu item to add.
         * \tparam Args Menu item arguments.
         * \param args Arguments to pass to menu item constructor.
         */
        template <typename T, typename... Args>
        inline void add_item(Args... args) {
            items.push_back(std::make_shared<T>(args...));
        };

    private:
        /*
         * Get menu items start iterator.
         */
        menu_item_citerator items_cbegin(void) const;

        /*
         * Get menu items end iterator.
         */
        menu_item_citerator items_cend(void) const;

        const std::string id;
        std::string title;
        menu_items items;
};

//! Shared pointer for addressing a menu.
typedef std::shared_ptr<mnu::menu> menu_sptr;
//! Constant shared pointer for addressing a menu.
typedef std::shared_ptr<const mnu::menu> menu_csptr;

}  // end namespace mnu

}  // end namespace wte

#endif
