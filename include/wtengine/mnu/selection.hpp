/*!
 * WTEngine | File:  selection.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_SELECTION_HPP
#define WTE_MNU_ITEM_SELECTION_HPP

#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <cassert>

#include "wtengine/mnu/item.hpp"

namespace wte::mnu {

/*!
 * \class selection
 * \brief Menu item with multiple options.
 */
class selection final : public item {
    public:
        /*!
         * \brief Menu item selection constructor.
         * \param label Item display label.
         * \param dvls Vector of display items.
         * \param vls Vector of allowed settings.
         * \param dfunc Function to determine defaut setting.
         * \param afunc Function to apply setting.
         */
        selection(
            const std::string& label,
            const std::vector<std::string>& dvls,
            const std::vector<std::string>& vls,
            const std::function<const std::string(void)>& dfunc,
            const std::function<void(const std::string&)>& afunc
        );

        ~selection() = default;  //!<  Default destructor.

        /*!
         * \brief Define left click process.
         */
        void on_left(void) override;

        /*!
         * \brief Define right click process.
         */
        void on_right(void) override;

        /*!
         * \brief Return display text for the menu item when rendering.
         * \return Vector of display text strings.
         */
        const std::vector<std::string> get_text(void) const override;

        /*!
         * \brief Reset to the default variable.
         */
        void reset_to_default(void) override;

        /*!
         * \brief Set the default variable.
         */
        void set_default(void) override;

        /*!
         * \brief Apply the setting.
         */
        void apply_setting(void) override;

        /*!
         * \brief Checks if the current selection is the same as the default.
         * \return True if the setting changed, false if it did not.
         */
        const bool setting_changed(void) const override;

    private:
        const std::vector<std::string> display_vals;
        const std::vector<std::string> vals;
        std::vector<std::string>::const_iterator current_val;
        std::vector<std::string>::const_iterator default_val;
        const std::function<const std::string(void)> defaulter;
        const std::function<void(const std::string&)> applier;
};

}  //  end namespace wte::mnu

#endif
