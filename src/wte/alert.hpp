/*
  WTEngine
  By:  Matthew Evans
  File:  alert.hpp

  See LICENSE.txt for copyright information

  Alerts
*/

#ifndef WTE_ALERT_HPP
#define WTE_ALERT_HPP

#include <allegro5/allegro.h>

#include <string>

namespace wte
{

//!  Alert class
/*!
  Allows the engine to set an alert flag to be displayed
*/
class alert {
    public:
        //!  Set the alert text and flag there is an alert
        inline static void set_alert(const std::string a) {
            alert_text = a;
            alert_set = true;
        };

        //!  Return the alert string
        inline static const std::string get_alert(void) { return alert_text; };

        //!  Check to see if an alert is set
        inline static const bool is_set(void) { return alert_set; };

        //!  Get the font color for alerts
        inline static const ALLEGRO_COLOR get_font_color(void) { return font_color; };
        //!  Get the background color for alerts
        inline static const ALLEGRO_COLOR get_bg_color(void) { return bg_color; };

        //!  Set the font color for alerts
        inline static void set_font_color(ALLEGRO_COLOR c) { font_color = c; };
        //!  Set the background color for alerts
        inline static void set_bg_color(ALLEGRO_COLOR c) { bg_color = c; };

        //!  Clear the alert
        inline static void clear(void) { alert_set = false; };

    private:
        static ALLEGRO_COLOR font_color;
        static ALLEGRO_COLOR bg_color;

        static std::string alert_text;
        static bool alert_set;
};

inline ALLEGRO_COLOR alert::font_color = {};
inline ALLEGRO_COLOR alert::bg_color = {};

inline std::string alert::alert_text = "";
inline bool alert::alert_set = false;

} //  end namespace wte

#endif
