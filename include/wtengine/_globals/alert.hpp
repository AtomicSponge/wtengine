/*!
 * WTEngine | File:  alert.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ALERT_HPP
#define WTE_ALERT_HPP

#include <string>

namespace wte {

/*!
 * \class alert
 * \brief Allows the engine to set an alert to be displayed to the screen.
 * 
 * Alerts are cleared after the player presses a button.
 */
class alert final {
    public:
        alert() = delete;
        ~alert() = delete;

        /*!
         * \brief Set the alert text and flag there is an alert.
         * \param a Alert to display.
         * \param l Location of alert.
         * \param t Time of alert.
         */
        static void set(
            const std::string& a,
            const std::string& l,
            const int64_t& t
        );

        /*!
         * \brief Return the alert.
         * \return The full alert message.
         */
        static const std::string get(void);

        /*!
         * \brief Return the alert text.
         * \return Description of the alert.
         */
        static const std::string get_text(void);

        /*!
         * \brief Return the alert location.
         * \return Location alert was made.
         */
        static const std::string get_location(void);

        /*!
         * \brief Return the alert time.
         * \return Time the alert was made.
         */
        static const int64_t get_time(void);

        /*!
         * \brief Check to see if an alert is set.
         * \return True if set, false if not set.
         */
        static const bool is_set(void);

        /*!
         * \brief Clear the alert.
         */
        static void clear(void);

    private:
        static std::string alert_text;      //  The alert display text.
        static std::string alert_location;  //  Location of the alert.
        static int64_t alert_time;          //  Alert time.
        static bool alert_set;              //  Flag to check if alert is set.
};

} //  end namespace wte

#endif
