/*!
 * WTEngine | File:  display.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_DISPLAY_HPP
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>

#include <string>
#include <stdexcept>
#include <cmath>

#include "wtengine/mgr/renderer.hpp"

namespace wte
{

/*!
 * \class display
 * \brief Handles control of the display.
 * 
 * Inherited by the main engine class.
 */
class display {
    public:
        /*!
         * \brief Display destructor.
         */
        inline virtual ~display() {};

        /*!
         * \brief Get the window title.
         * 
         * \return Window title as string.
         */
        const std::string get_window_title(void) const;

    protected:
        /*!
         * \brief Display constructor.
         * 
         * \param title Window title.
         */
        display(const std::string& title);

        /*!
         * \brief Configure the display.
         */
        void create_display(void);

        /*!
         * \brief Destroy the display.
         */
        void destroy_display(void);

        /*!
         * \brief Reconfigure the display.
         */
        void reconf_display(void);

        //!  Allegro object for the display.
        ALLEGRO_DISPLAY* _display;

    private:
        std::string window_title;  //  Title for application window.
};

} //  end namespace wte

#endif
