/*!
 * WTEngine | File:  notice.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_NOTICE_HPP
#define WTE_NOTICE_HPP

#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"

namespace wte::mgr {
    class renderer;
}

namespace wte {

/*!
 * \class notice
 * \brief Allows the engine to set a notice to be displayed to the screen.
 * 
 * Notices are cleared after the player presses a button.
 */
class notice final {
    friend class input;
    friend class mgr::renderer;

    public:
        notice() = delete;                       //!<  Delete constructor.
        ~notice() = delete;                      //!<  Delete destructor.
        notice(const notice&) = delete;          //!<  Delete copy constructor.
        void operator=(notice const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Set the notice and flag that its set.
         * \param n Notice to display.
         */
        static void set(
            const std::string& n
        );

        /*!
         * \brief Return the notice.
         * \return The notice message.
         */
        static const std::string get(void);

        /*!
         * \brief Check to see if a notice is set.
         * \return True if set, false if not set.
         */
        static const bool is_set(void);

        /*!
         * \brief Set the notice font.
         * \param a Asset to use for font.
         */
        static void set_notice_font(const wte_asset<al_font> a);

        /*!
         * \brief Set the notice font color.
         * \param c Allegro color to use for font.
         */
        static void set_notice_font_color(const ALLEGRO_COLOR& c);

        /*!
         * \brief Set the notice background color.
         * \param c Allegro color to use for background.
         */
        static void set_notice_bg_color(const ALLEGRO_COLOR& c);

    private:
        /*
         * Clear the notice.
         */
        static void clear(void);

        /*
         * Get the notice font.
         */
        static const wte_asset<al_font> get_notice_font(void);

        /*
         * Get the notice font color.
         */
        static const ALLEGRO_COLOR get_notice_font_color(void);

        /*
         * Get the notice background color.
         */
        static const ALLEGRO_COLOR get_notice_bg_color(void);

        static std::string notice_text;          //  The notice display text.
        static bool notice_set;                  //  Flag to check if a notice is set.
        static wte_asset<al_font> notice_font;   //  Font for notice.
        static ALLEGRO_COLOR notice_font_color;  //
        static ALLEGRO_COLOR notice_bg_color;    //
};

}  //  end namespace wte

#endif
