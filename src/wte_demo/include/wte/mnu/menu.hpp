/*!
 * \brief WTEngine | File:  menu.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Menu object.
 */

#ifndef WTE_MENU_MENU_HPP
#define WTE_MENU_MENU_HPP

#include <string>
#include <vector>

#include "menu_item.hpp"

namespace wte
{

namespace mnu
{

typedef std::vector<menu_item_sptr> menu_items;
typedef std::vector<menu_item_sptr>::iterator menu_item_iterator;
typedef std::vector<menu_item_sptr>::const_iterator menu_item_citerator;

/*!
 *
 */
class menu {
    public:
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu(const std::string i, const std::string t) : id(i), title(t) { items.clear(); };
        
        /*!
         * Description
         * \param void
         * \return void
         */
        inline ~menu() { items.clear(); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_id(void) const { return id; };
        
        /*!
         * Description
         * \param void
         * \return void
         */
        inline const std::string get_title(void) const { return title; };
        
        /*!
         * Description
         * \param void
         * \return void
         */
        //inline menu_items get_items(void) const { return items; };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline std::size_t num_items(void) const { return items.size(); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_citerator items_begin() const { return items.begin(); };
        
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_citerator items_end() const { return items.end(); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_citerator items_cbegin() const { return items.cbegin(); };
        
        /*!
         * Description
         * \param void
         * \return void
         */
        inline menu_item_citerator items_cend() const { return items.cend(); };

        /*!
         * Description
         * \param void
         * \return void
         */
        inline void set_title(const std::string t) { title = t; };

        /*!
         * Add a menu item to an existing menu
         * \param void
         * \return void
         */
        inline const bool add_item(const menu_item_sptr item) {
            items.push_back(item);
            return true;
        };

    private:
        menu_items items;

        std::string id;
        std::string title;
};

}  // end namespace mnu

}  // end namespace wte

#endif
