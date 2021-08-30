/*!
 * WTEngine | File:  display.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_DISPLAY_HPP
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>

#include <string>
#include <stdexcept>
#include <cmath>

#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/mgr/renderer.hpp"

namespace wte {

/*!
 * \class display
 * \brief Handles control of the display.
 */
class display {
    friend class engine;

    public:
        virtual ~display() = default;             //!<  Default virtual destructor.
        display(const display&) = delete;         //!<  Delete copy constructor.
        void operator=(display const&) = delete;  //!<  Delete assignment operator.

        /*!
         * \brief Get the window title.
         * \return Window title as string.
         */
        const std::string get_window_title(void) const;

    protected:
        /*!
         * \brief Display constructor.
         * \param title Window title.
         */
        display(const std::string& title);

    private:
        /*
         * Configure the display.
         */
        void create_display(void);

        /*
         * Destroy the display.
         */
        void destroy_display(void);

        /*
         * Reconfigure the display.
         */
        void reconf_display(void);

        static ALLEGRO_DISPLAY* _display;  //!<  Allegro object for the display.

        std::string window_title;  //  Title for application window.
        static bool initialized;   //  Restrict to one instance.
};

} //  end namespace wte

#endif
