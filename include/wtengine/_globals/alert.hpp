/*!
 * WTEngine | File:  alert.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_ALERT_HPP
#define WTE_ALERT_HPP

#include <string>

#include <allegro5/allegro.h>

namespace wte
{

/*!
 * \class alert
 * \brief Allows the engine to set an alert to be displayed to the screen.
 * 
 * Alerts are cleared after the player presses a button.
 */
class alert {
    public:
        /*!
         * \brief Set the alert text and flag there is an alert.
         * 
         * \param a Alert to display.
         */
        inline static void set_alert(const std::string& a) {
            alert_text = a;
            alert_set = true;
        };

        /*!
         * \brief Return the alert string.
         * 
         * \return The alert message itself.
         */
        inline static const std::string get_alert(void) { return alert_text; };

        /*!
         * \brief Check to see if an alert is set.
         * 
         * \return True if set, false if not set.
         */
        inline static const bool is_set(void) { return alert_set; };

        /*!
         * \brief Get the font color for alerts.
         * 
         * \return A struct representing an Allegro Color object for the alert font.
         */
        inline static const ALLEGRO_COLOR get_font_color(void) { return font_color; };

        /*!
         * \brief Get the background color for alerts.
         * 
         * \return A struct representing an Allegro Color object for the alert background.
         */
        inline static const ALLEGRO_COLOR get_bg_color(void) { return bg_color; };

        /*!
         * \brief Set the font color for alerts.
         * 
         * \param c A struct representing an Allegro Color object for the alert font.
         */
        inline static void set_font_color(const ALLEGRO_COLOR c) { font_color = c; };

        /*!
         * \brief Set the background color for alerts.
         * 
         * \param c A struct representing an Allegro Color object for the alert background.
         */
        inline static void set_bg_color(const ALLEGRO_COLOR c) { bg_color = c; };

        /*!
         * \brief Clear the alert.
         */
        inline static void clear(void) { alert_set = false; };

    private:
        inline alert() {};
        inline ~alert() {};

        inline static ALLEGRO_COLOR font_color = {};
        inline static ALLEGRO_COLOR bg_color = {};

        inline static std::string alert_text = "";
        inline static bool alert_set = false;
};

} //  end namespace wte

#endif
