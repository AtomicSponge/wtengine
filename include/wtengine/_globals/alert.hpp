/*!
 * WTEngine | File:  alert.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
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
        static void set(const std::string& a);

        /*!
         * \brief Return the alert string.
         * 
         * \return The alert message itself.
         */
        static const std::string get_alert(void);

        /*!
         * \brief Check to see if an alert is set.
         * 
         * \return True if set, false if not set.
         */
        static const bool is_set(void);

        /*!
         * \brief Get the font color for alerts.
         * 
         * \return A struct representing an Allegro Color object for the alert font.
         */
        static const ALLEGRO_COLOR get_font_color(void);

        /*!
         * \brief Get the background color for alerts.
         * 
         * \return A struct representing an Allegro Color object for the alert background.
         */
        static const ALLEGRO_COLOR get_bg_color(void);

        /*!
         * \brief Set the font color for alerts.
         * 
         * \param c A struct representing an Allegro Color object for the alert font.
         */
        static void set_font_color(const ALLEGRO_COLOR& c);

        /*!
         * \brief Set the background color for alerts.
         * 
         * \param c A struct representing an Allegro Color object for the alert background.
         */
        static void set_bg_color(const ALLEGRO_COLOR& c);

        /*!
         * \brief Clear the alert.
         */
        static void clear(void);

    private:
        alert();
        ~alert();

        static ALLEGRO_COLOR font_color;
        static ALLEGRO_COLOR bg_color;

        static std::string alert_text;
        static bool alert_set;
};

} //  end namespace wte

#endif
