/*!
 * \brief WTEngine | File:  alert.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Alerts.
 */

#ifndef WTE_ALERT_HPP
#define WTE_ALERT_HPP

#include <allegro5/allegro.h>

#include <string>

namespace wte
{

//!  Alerts
/*!
 * Allows the engine to set an alert to be displayed to the screen.
 * Alerts are cleared after the player presses a button.
 */
class alert {
    public:
        /*!
         * Set the alert text and flag there is an alert.
         * \param a Alert to display.
         * \return void
         */
        inline static void set_alert(const std::string a) {
            alert_text = a;
            alert_set = true;
        };

        /*!
         * Return the alert string
         * \param void
         * \return The alert message itself.
         */
        inline static const std::string get_alert(void) { return alert_text; };

        /*!
         * Check to see if an alert is set.
         * \param void
         * \return True if set, false if not set.
         */
        inline static const bool is_set(void) { return alert_set; };

        /*!
         * Get the font color for alerts.
         * \param void
         * \return A struct representing an Allegro Color object for the alert font.
         */
        inline static const ALLEGRO_COLOR get_font_color(void) { return font_color; };

        /*!
         * Get the background color for alerts.
         * \param void
         * \return A struct representing an Allegro Color object for the alert background.
         */
        inline static const ALLEGRO_COLOR get_bg_color(void) { return bg_color; };

        /*!
         * Set the font color for alerts.
         * \param c A struct representing an Allegro Color object for the alert font.
         * \return void
         */
        inline static void set_font_color(const ALLEGRO_COLOR c) { font_color = c; };

        /*!
         * Set the background color for alerts.
         * \param c A struct representing an Allegro Color object for the alert background.
         * \return void
         */
        inline static void set_bg_color(const ALLEGRO_COLOR c) { bg_color = c; };

        /*!
         * Clear the alert.
         * \param void
         * \return void
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
