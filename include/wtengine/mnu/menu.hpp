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
#include "wtengine/mnu/item.hpp"

namespace wte::mgr {
    class menus;
}

namespace wte::mnu {

//!  Container for storing menu items.
typedef std::vector<item_sptr> items;
//!  Constant iterator for addressing menu items.
typedef std::vector<item_sptr>::const_iterator item_citerator;

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
            const std::string& i,
            const std::string& t
        );

        ~menu() = default;  //!<  Default destructor.

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
            _items.push_back(std::make_shared<T>(args...));
        };

    private:
        /*
         * Get menu items start iterator.
         */
        item_citerator items_cbegin(void) const;

        /*
         * Get menu items end iterator.
         */
        item_citerator items_cend(void) const;

        /*
         * Get menu items.
         */
        items get_items(void) const;

        const std::string id;
        std::string title;
        items _items;
};

//! Shared pointer for addressing a menu.
typedef std::shared_ptr<mnu::menu> menu_sptr;
//! Constant shared pointer for addressing a menu.
typedef std::shared_ptr<const mnu::menu> menu_csptr;

}  //  end namespace wte::mnu

#endif
