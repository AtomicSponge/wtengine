/*!
 * WTEngine | File:  item.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_HPP
#define WTE_MNU_ITEM_HPP

#include <string>
#include <vector>
#include <memory>

#include "wtengine/_globals/message.hpp"

namespace wte::mgr {
    class menus;
}

namespace wte::mnu {

/*!
 * \class item
 * \brief Menu item interface class.
 */
class item {
    friend class mgr::menus;

    public:
        inline virtual ~item() = default;    //!<  Default virtual destructor.

        /*!
         * \brief Get the display label.
         * \return The label text.
         */
        inline const std::string get_label(void) const { return label; };

    protected:
        /*!
         * \brief Menu item constructor.
         * \param l Label of the menu item.
         */
        inline item(
            const std::string& l
        ) : label(l) {};

    private:
        /*
         * Return display text for the menu item when rendering.
         */
        inline virtual const std::vector<std::string> get_text(void) const { return {"null"}; };

        /*
         * Define left click process.
         */
        inline virtual void on_left(void) {};

        /*
         * Define right click process.
         */
        inline virtual void on_right(void) {};

        /*
         * Define selection process.
         */
        inline virtual void on_select(void) {};

        /*
         * Define reset process.
         */
        inline virtual void reset_to_default(void) {};

        /*
         * Define default value process.
         */
        inline virtual void set_default(void) {};

        /*
         * Define apply setting process.
         */
        inline virtual void apply_setting(void) {};

        /*
         * Define setting changed process.
         */
        inline virtual const bool setting_changed(void) const { return false; };

        const std::string label;
};

//! Menu Item shared pointer
typedef std::shared_ptr<item> item_sptr;

}  //  end namespace wte::mnu

#endif
