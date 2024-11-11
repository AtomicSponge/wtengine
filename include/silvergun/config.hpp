/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CONFIG_HPP)
#define WTE_CONFIG_HPP

#include <allegro5/allegro.h>

#include <fstream>

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/_defines.hpp"

namespace wte::mgr {
  class audio;
}

namespace wte::mgr::gfx {
  class renderer;
}

namespace wte {

/*!
 * \class config
 * \brief Store engine config variables in a globally accessible class.
 */
class config {
  friend class display;
  friend class engine;
  friend class input;
  friend class mgr::audio;
  friend class mgr::gfx::renderer;

  private:
    struct _flags {
      inline static bool is_running = true;
      inline static bool record_input = false;
      inline static bool keyboard_installed = false;
      inline static bool mouse_installed = false;
      inline static bool joystick_installed = false;
      inline static bool touch_installed = false;
      inline static bool audio_installed = false;
      inline static bool show_hitboxes = false;
    };

    struct _volume {
      inline static float main = 1.0f;
      inline static float music = 1.0f;
      inline static float music_a = 1.0f;
      inline static float music_b = 1.0f;
      inline static float sample = 1.0f;
      inline static float voice = 1.0f;
      inline static float ambiance = 1.0f;
    };

    struct _gfx {
      inline static int screen_w = 0, screen_h = 0;
      inline static int viewport_w = 0, viewport_h = 0;
      inline static std::size_t vsync = 2;
      inline static std::size_t display_mode = 0;
      inline static float scale_factor = 1.0f;
    };

    inline static bool initialized = false;  //  Restrict to one instance.

  protected:
    config() {
      if (initialized == true)
        throw engine_error("Config instance already running!");
      initialized = true;
    };

  public:
    virtual ~config() = default;             //  Default virtual destructor.
    config(const config&) = delete;          //  Delete copy constructor.
    void operator=(config const&) = delete;  //  Delete assignment operator.

    /*!
     * \struct flags
     * \brief Engine flags.
     */
    struct flags {
      inline static const bool& is_running = _flags::is_running;                  //!<  Flag for if the program itself is running.
      inline static bool engine_paused = false;                                   //!<  Flag to check if the engine if paused.
      inline static const bool& record_input = _flags::record_input;              //!<  Flag to enable/disable input recording.
      inline static const bool& keyboard_installed = _flags::keyboard_installed;  //!<  Flag to check if a keyboard is installed.
      inline static const bool& mouse_installed = _flags::mouse_installed;        //!<  Flag to check if a mouse is installed.
      inline static const bool& joystick_installed = _flags::joystick_installed;  //!<  Flag to check if a joystick is installed.
      inline static const bool& touch_installed = _flags::touch_installed;        //!<  Flag to check if touch input is installed.
      inline static const bool& audio_installed = _flags::audio_installed;        //!<  Flag to check if audio was installed.
      inline static bool draw_fps = true;                                         //!<  Flag to check if fps should be drawn.
      inline static bool input_enabled = true;                                    //!<  Flag to check if game input is enabled.
      inline static const bool& show_hitboxes = _flags::show_hitboxes;            //!<  Flag to enable/disable hitbox rendering.
    };

    /*!
     * \struct volume
     * \brief Volume levels.
     */
    struct volume {
      inline static const float& main = _volume::main;          //!<  Main volume level.
      inline static const float& music = _volume::music;        //!<  Mix 1 volume level.
      inline static const float& music_a = _volume::music_a;    //!<  Mix 1a volume level.
      inline static const float& music_b = _volume::music_b;    //!<  Mix 1b volume level.
      inline static const float& sample = _volume::sample;      //!<  Mix 2 volume level.
      inline static const float& voice = _volume::voice;        //!<  Mix 3 volume level.
      inline static const float& ambiance = _volume::ambiance;  //!<  Mix 4 volume level.
    };

    /*!
     * \struct gfx
     * \brief Graphics settings.
     */
    struct gfx {
      inline static const int& screen_w = _gfx::screen_w;                  //!<  Screen width.
      inline static const int& screen_h = _gfx::screen_h;                  //!<  Screen height.
      inline static const int& viewport_w = _gfx::viewport_w;              //!<  Viewport width.
      inline static const int& viewport_h = _gfx::viewport_h;              //!<  Viewport height.
      inline static const std::size_t& vsync = _gfx::vsync;                //!<  Vsync setting.
      inline static const std::size_t& display_mode = _gfx::display_mode;  //!<  Display mode setting.
      inline static const float& scale_factor = _gfx::scale_factor;        //!<  Arena scale factor.
    };
};

}  //  end namespace wte

#endif
