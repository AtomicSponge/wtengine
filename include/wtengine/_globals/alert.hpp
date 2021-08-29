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

namespace mgr {
    class spawner;
    class systems;
    class renderer;
}

/*!
 * \class alert
 * \brief Allows the engine to set an alert to be displayed to the screen.
 * 
 * Alerts are cleared after the player presses a button.
 * Used internally by the engine only.  Use notice class instead.
 */
class alert final {
    friend class commands;
    friend class engine;
    friend class input;
    friend class mgr::spawner;
    friend class mgr::systems;
    friend class mgr::renderer;

    public:
        alert() = delete;
        ~alert() = delete;

    private:
        /*
         * Set the alert text and flag there is an alert.
         */
        static void set(
            const std::string& a,
            const std::string& l,
            const int64_t& t
        );

        /*
         * Return the alert.
         */
        static const std::string get(void);

        /*
         * Return the alert description.
         */
        static const std::string get_text(void);

        /*
         * Return the alert location.
         */
        static const std::string get_location(void);

        /*
         * Return the alert time.
         */
        static const int64_t get_time(void);

        /*
         * Check to see if an alert is set.
         */
        static const bool is_set(void);

        /*
         * Clear the alert.
         */
        static void clear(void);

        static std::string alert_text;      //  The alert display text.
        static std::string alert_location;  //  Location of the alert.
        static int64_t alert_time;          //  Alert time.
        static bool alert_set;              //  Flag to check if alert is set.
};

} //  end namespace wte

#endif
