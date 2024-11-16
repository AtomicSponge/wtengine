/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_DISPLAY_HPP)
#define SLV_DISPLAY_HPP

#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include <string>

#include "silvergun/_debug/exceptions.hpp"
#include "silvergun/_globals/_defines.hpp"
#include "silvergun/mgr/renderer.hpp"

namespace slv {

/*!
 * \class display
 * \brief Handles control of the display.
 */
class display {
  friend class engine;

  public:
    virtual ~display() = default;             //  Default virtual destructor.
    display(const display&) = delete;         //  Delete copy constructor.
    void operator=(display const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Set the window title.
     * \param title Window title.
     */
    static void set_window_title(const std::string& title) {
      window_title = title;
    };

    /*!
     * \brief Set the scale factor.
     * \param f Scale factor value.
     */
    static void set_scale_factor(float f) {
      if (f < 0.5f || f > 2.0f) f = 1.0f;
      config::_gfx::scale_factor = f;
    };

    /*!
     * \brief Change the display mode.  Requires engine restart.
     * \param m New display mode.
     */
    static void set_display_mode(std::size_t m) {
      if (m > 1) m = 1;
      config::_gfx::display_mode = m;
    };

    /*!
     * \brief Set the vsync setting.
     * \param v New vsync setting.
     */
    static void set_vsync(std::size_t v) {
      if (v > 2) v = 0;
      config::_gfx::vsync = v;
    };

    /*!
     * \brief Resize the display.
     * \param w Screen width.
     * \param h Screen height.
     */
    static void resize_display(int w, int h) {
      if (w < 1) w = 1;
      if (h < 1) h = 1;
      config::_gfx::screen_w = w;
      config::_gfx::screen_h = h;
      al_resize_display(_display, config::gfx::screen_w, config::gfx::screen_h);
      if (!al_acknowledge_resize(_display))
        throw engine_error("Failed to resize display!");
    };

  protected:
    display() {
      if (initialized == true)
        throw engine_error("Display instance already running!");
      initialized = true;
    };

  private:
    //  Configure the display.
    static void create_display(int w, int h) {
      //  Set a default screen size
      config::_gfx::screen_w = w;
      config::_gfx::screen_h = h;

      al_reset_new_display_options();

      //  Configure vsync options.  Gfx driver may override this.
      //  0 - System decides | 1 - VSync on | 2 - VSync off
      if (config::gfx::vsync >= 0 && config::gfx::vsync <= 2) {
        al_set_new_display_option(ALLEGRO_VSYNC, config::gfx::vsync, ALLEGRO_SUGGEST);
      } else {
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
      }

      //  Check if a display mode is set.
      if (config::gfx::display_mode == 1) {
        (build_options.opengl_latest ?
          al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_FULLSCREEN_WINDOW):
          al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN_WINDOW));
      } else {
        (build_options.opengl_latest ?
          al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
          al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
      }

      //  Create the display.  Full screen windowed defaults to the display resolution.
      _display = al_create_display(config::gfx::screen_w, config::gfx::screen_h);

      //  Display failed to load, try a fallback.
      if (!_display) {
        (build_options.opengl_latest ?
          al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
          al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
        _display = al_create_display(config::gfx::screen_w, config::gfx::screen_h);
        if (!_display) throw engine_error("Failed to configure display!");
        config::_gfx::display_mode = 0;
        config::_gfx::scale_factor = 1.0f;
      }

      //  Set window title.
      al_set_window_title(_display, window_title.c_str());

      //  Set window icon.
      ALLEGRO_FILE* file;
      file = al_fopen("icon.bmp", "rb");
      if (file) {
        ALLEGRO_BITMAP* icon_bitmap = al_load_bitmap_f(file, ".bmp");
        al_set_display_icon(_display, icon_bitmap);
        al_destroy_bitmap(icon_bitmap);
      }
      al_fclose(file);
    };

    //  Destroy the display.
    static void destroy_display(void) {
      al_destroy_display(_display);
    };

    //  Title for application window.
    inline static std::string window_title = "Silvergun Game Engine";
    inline static bool initialized = false;  //  Restrict to one instance.

    inline static ALLEGRO_DISPLAY* _display;  //  Allegro object for the display.
};

}

#endif
