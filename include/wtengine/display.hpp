/*!
 * wtengine | File:  display.hpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#if !defined(WTE_DISPLAY_HPP)
#define WTE_DISPLAY_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include <cmath>
#include <string>
#include <map>
#include <tuple>
#include <utility>
#include <type_traits>

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/mgr/renderer.hpp"

#if WTE_USE_IMGUI
#include "imgui/imgui.h"
#include "imgui/imgui_impl_allegro5.h"
#endif

namespace wte {

typedef struct WTE_DISPLAY_RESOLUTION {
    std::string label;
    int width;
    int height;
} WTE_DISPLAY_RESOLUTION;

typedef std::vector<WTE_DISPLAY_RESOLUTION> display_results;

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
         * \brief Set the scale factor.
         * \param f Scale factor value.
         */
        static void set_scale_factor(float f);

        /*!
         * \brief Change the display mode.  Requires engine restart.
         * \param m New display mode.
         */
        static void set_display_mode(std::size_t m);

        /*!
         * \brief Set the screen size.  Call during engine setup.
         * \param w Screen width.
         * \param h Screen height.
         */
        static void set_screen_size(int w, int h);

        /*!
         * \brief Set the vsync setting.
         * \param v New vsync setting.
         */
        static void set_vsync(std::size_t v);

        /*!
         * \brief Resize the display.
         * \param w Screen width.
         * \param h Screen height.
         */
        static void resize_display(int w, int h);

        /*!
         * \brief
         */
        static void add_res_check(ALLEGRO_DISPLAY_MODE mode);

        /*!
         * \brief
         */
        static const display_results get_available_res(void);

    protected:
        display();

    private:
        static void create_display(void);   //  Configure the display.
        static void destroy_display(void);  //  Destroy the display.
        static std::string window_title;    //  Title for application window.
        static bool initialized;            //  Restrict to one instance.

        static ALLEGRO_DISPLAY* _display;   //  Allegro object for the display.

        //  Default list of resolutions the engine will check for
        inline static std::vector<WTF_DISPLAY_MODE> check_res = {
            // 4:3
            WTF_DISPLAY_MODE { 1280, 960, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 1600, 1200, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 3200, 2400, WTF_PIXEL_FORMAT_ANY, 0 },
            // 16:9
            WTF_DISPLAY_MODE { 1920, 1080, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 2560, 1440, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 3840, 2160, WTF_PIXEL_FORMAT_ANY, 0 },
            // 21:9
            WTF_DISPLAY_MODE { 2520, 1080, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 3440, 1440, WTF_PIXEL_FORMAT_ANY, 0 },
            WTF_DISPLAY_MODE { 5120, 2160, WTF_PIXEL_FORMAT_ANY, 0 }
        };
};

}  //  end namespace wte

#endif
