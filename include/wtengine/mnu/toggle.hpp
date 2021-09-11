/*!
 * WTEngine | File:  toggle.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MNU_ITEM_TOGGLE_HPP
#define WTE_MNU_ITEM_TOGGLE_HPP

#include <string>
#include <vector>
#include <functional>

#include "wtengine/mnu/item.hpp"

namespace wte::mnu {

/*!
 * \class toggle
 * \brief On/off menu item.
 */
class toggle final : public item {
    public:
        /*!
         * \brief Create a menu toggle item.  Uses default setting type.
         * \param label Item display label. 
         * \param dfunc Function to determine defaut setting.
         * \param afunc Function to apply setting.
         */
        toggle(
            const std::string& label,
            const std::function<const bool(void)>& dfunc,
            const std::function<void(const bool&)>& afunc
        );

        ~toggle() = default;  //!<  Default destructor.

        /*!
         * \brief Toggle on.
         */
        void on_left(void) override;

        /*!
         * \brief Toggle off.
         */
        void on_right(void) override;

        /*!
         * \brief Get display text
         * \return Vector of strings to render.
         */
        const std::vector<std::string> get_text(void) const override;

        /*!
         * \brief Reset to default value.
         */
        void reset_to_default(void) override;

        /*!
         * \brief Determine and set the default value for the menu toggle item.
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
        bool toggled, default_val;
        const std::function<const bool(void)> defaulter;
        const std::function<void(const bool&)> applier;
};

}  //  end namespace wte::mnu

#endif
