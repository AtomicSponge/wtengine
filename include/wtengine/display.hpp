/*!
 * wtengine | File:  display.hpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#ifndef WTE_DISPLAY_HPP
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include <cmath>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <type_traits>
#include <stdexcept>

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
         * \brief Set the window title.
         * \param title Window title.
         */
        static void set_window_title(const std::string& title);

        /*!
         * \brief Set the display mode.
         * \param m Display mode value.
         */
        static void set_display_mode(const std::size_t& m);

        /*!
         * \brief Set the scale factor.
         * \param f Scale factor value.
         */
        static void set_scale_factor(const float& f);

    protected:
        display();

    private:
        void create_display(void);         //  Configure the display.
        void destroy_display(void);        //  Destroy the display.
        void resize_display(               //  Resize the display.
            const int& width, const int& height);
        static std::string window_title;   //  Title for application window.
        static bool initialized;           //  Restrict to one instance.
        static ALLEGRO_DISPLAY* _display;  //  Allegro object for the display.
        constexpr static bool opengl_latest = build_options.opengl_latest;
};

}  //  end namespace wte

#endif
