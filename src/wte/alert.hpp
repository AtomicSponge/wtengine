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

#include "wte_global_defines.hpp"

namespace wte
{

//!
/*!
*/
class alert {
    public:
        //!
        inline static void set_alert(const std::string a) {
            alert_text = a;
            alert_set = true;
        };

        //!
        inline static const std::string get_alert(void) { return alert_text; };

        //!
        inline static const bool is_set(void) { return alert_set; };

        //!
        inline static const ALLEGRO_COLOR get_font_color(void) { return font_color; };
        //!
        inline static const ALLEGRO_COLOR get_bg_color(void) { return bg_color; };

        //!
        inline static void clear(void) { alert_set = false; };

    private:
        static ALLEGRO_COLOR font_color;
        static ALLEGRO_COLOR bg_color;

        static std::string alert_text;
        static bool alert_set;
};

inline ALLEGRO_COLOR alert::font_color = WTE_COLOR_WHITE;
inline ALLEGRO_COLOR alert::bg_color = WTE_COLOR_RED;

inline std::string alert::alert_text = "";
inline bool alert::alert_set = false;

} //  end namespace wte

#endif
