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

namespace wte {

/*!
 * \class notice
 * \brief Allows the engine to set a notice to be displayed to the screen.
 * 
 * Notices are cleared after the player presses a button.
 */
class notice final {
    public:
        notice() = delete;
        ~notice() = delete;

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
         * \brief Clear the notice.
         */
        static void clear(void);

        /*!
         * \brief Set notice background.
         * \param a Asset to use for background.
         */
        static void set_notice_bg(wte_asset<al_bitmap> a);

        /*!
         * \brief Set notice font.
         * \param a Asset to use for font.
         */
        static void set_notice_font(wte_asset<al_font> a);

    private:
        static std::string notice_text;         //  The notice display text.
        static bool notice_set;                 //  Flag to check if a notice is set.
        static wte_asset<al_bitmap> notice_bg;  //  Background for notice.
        static wte_asset<al_font> notice_font;  //  Font for notice.
};

} //  end namespace wte

#endif
