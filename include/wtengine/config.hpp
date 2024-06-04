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

  public:
    virtual ~config() = default;                  //  Default virtual destructor.
    config(const config&) = delete;               //  Delete copy constructor.
    void operator=(config const&) = delete;       //  Delete assignment operator.

    /*!
     * \struct flags
     * \brief Engine flags.
     */
    struct flags {
      static const bool& is_running;             //!<  Flag for if the program itself is running.
      static const bool& engine_started;         //!<  Flag to check if the engine is running.
      inline static bool engine_paused = false;  //!<  Flag to check if the engine if paused.
      static const bool& record_input;           //!<  Flag to enable/disable input recording.
      static const bool& keyboard_installed;     //!<  Flag to check if a keyboard is installed.
      static const bool& mouse_installed;        //!<  Flag to check if a mouse is installed.
      static const bool& joystick_installed;     //!<  Flag to check if a joystick is installed.
      static const bool& touch_installed;        //!<  Flag to check if touch input is installed.
      static const bool& audio_installed;        //!<  Flag to check if audio was installed.
      inline static bool draw_fps = true;        //!<  Flag to check if fps should be drawn.
      inline static bool input_enabled = true;   //!<  Flag to check if game input is enabled.
      static const bool& show_hitboxes;          //!<  Flag to enable/disable hitbox rendering.
    };

    /*!
     * \struct volume
     * \brief Volume levels.
     */
    struct volume {
      static const float& main;                 //!<  Main volume level.
      static const float& music;                //!<  Mix 1 volume level.
      static const float& music_a;              //!<  Mix 1a volume level.
      static const float& music_b;              //!<  Mix 1b volume level.
      static const float& sample;               //!<  Mix 2 volume level.
      static const float& voice;                //!<  Mix 3 volume level.
      static const float& ambiance;             //!<  Mix 4 volume level.
    };

    /*!
     * \struct gfx
     * \brief Graphics settings.
     */
    struct gfx {
      static const int& screen_w;               //!<  Screen width.
      static const int& screen_h;               //!<  Screen height.
      static const int& viewport_w;             //!<  Viewport width.
      static const int& viewport_h;             //!<  Viewport height.
      static const std::size_t& vsync;          //!<  Vsync setting.
      static const std::size_t& display_mode;   //!<  Display mode setting.
      static const float& scale_factor;         //!<  Arena scale factor.
    };

    /*!
     * \struct controls
     * \brief Control binding settings.
     */
    struct controls {
      inline static int key_menu = WTE_KEY_PAUSE;

      /*
       * Player 1 keyboard
       */
      inline static int p1_key_up = 0;
      inline static int p1_key_down = 0;
      inline static int p1_key_left = 0;
      inline static int p1_key_right = 0;
      inline static int p1_key_action1 = 0;
      inline static int p1_key_action2 = 0;
      inline static int p1_key_action3 = 0;
      inline static int p1_key_action4 = 0;
      inline static int p1_key_action5 = 0;
      inline static int p1_key_action6 = 0;
      inline static int p1_key_action7 = 0;
      inline static int p1_key_action8 = 0;

      /*
       * Player 1 joystick
       */
      inline static int p1_button_up = 0;
      inline static int p1_button_down = 0;
      inline static int p1_button_left = 0;
      inline static int p1_button_right = 0;
      inline static int p1_button_action1 = 0;
      inline static int p1_button_action2 = 0;
      inline static int p1_button_action3 = 0;
      inline static int p1_button_action4 = 0;
      inline static int p1_button_action5 = 0;
      inline static int p1_button_action6 = 0;
      inline static int p1_button_action7 = 0;
      inline static int p1_button_action8 = 0;
      inline static int p1_button_start = 0;
      inline static int p1_button_select = 0;
      inline static float p1_pola_x = 0.0f;
      inline static float p1_pola_y = 0.0f;
      inline static float p1_polb_x = 0.0f;
      inline static float p1_polb_y = 0.0f;
      inline static float p1_polc_x = 0.0f;
      inline static float p1_polc_y = 0.0f;
      inline static float p1_throttle_left = 0.0f;
      inline static float p1_throttle_right = 0.0f;

      /*
       * Player 2 keyboard
       */
      inline static int p2_key_up = 0;
      inline static int p2_key_down = 0;
      inline static int p2_key_left = 0;
      inline static int p2_key_right = 0;
      inline static int p2_key_action1 = 0;
      inline static int p2_key_action2 = 0;
      inline static int p2_key_action3 = 0;
      inline static int p2_key_action4 = 0;
      inline static int p2_key_action5 = 0;
      inline static int p2_key_action6 = 0;
      inline static int p2_key_action7 = 0;
      inline static int p2_key_action8 = 0;

      /*
       * Player 2 joystick
       */
      inline static int p2_button_up = 0;
      inline static int p2_button_down = 0;
      inline static int p2_button_left = 0;
      inline static int p2_button_right = 0;
      inline static int p2_button_action1 = 0;
      inline static int p2_button_action2 = 0;
      inline static int p2_button_action3 = 0;
      inline static int p2_button_action4 = 0;
      inline static int p2_button_action5 = 0;
      inline static int p2_button_action6 = 0;
      inline static int p2_button_action7 = 0;
      inline static int p2_button_action8 = 0;
      inline static int p2_button_start = 0;
      inline static int p2_button_select = 0;
      inline static float p2_pola_x = 0.0f;
      inline static float p2_pola_y = 0.0f;
      inline static float p2_polb_x = 0.0f;
      inline static float p2_polb_y = 0.0f;
      inline static float p2_polc_x = 0.0f;
      inline static float p2_polc_y = 0.0f;
      inline static float p2_throttle_left = 0.0f;
      inline static float p2_throttle_right = 0.0f;
  };

  /*!
   * \brief Load engine config settings from file.
   * \return False on fail, true on success.
   */
  static bool load(void) {
    std::ifstream dfile("settings.cfg", std::ios::binary);
    //  Data file doesn't exist, create one
    if(!dfile.good()) {
      if(!save()) return false;
      dfile.open("settings.cfg", std::ios::binary);
    }
    if(!dfile.good()) return false;

    try {
      dfile.read(reinterpret_cast<char*>(&flags::draw_fps), sizeof flags::draw_fps);
      dfile.read(reinterpret_cast<char*>(&_gfx::vsync), sizeof _gfx::vsync);
      dfile.read(reinterpret_cast<char*>(&_gfx::display_mode), sizeof _gfx::display_mode);
      dfile.read(reinterpret_cast<char*>(&_gfx::scale_factor), sizeof _gfx::scale_factor);
      dfile.read(reinterpret_cast<char*>(&_gfx::screen_w), sizeof _gfx::screen_w);
      dfile.read(reinterpret_cast<char*>(&_gfx::screen_h), sizeof _gfx::screen_h);
      dfile.read(reinterpret_cast<char*>(&_volume::main), sizeof _volume::main);
      dfile.read(reinterpret_cast<char*>(&_volume::music), sizeof _volume::music);
      dfile.read(reinterpret_cast<char*>(&_volume::sample), sizeof _volume::sample);
      dfile.read(reinterpret_cast<char*>(&_volume::voice), sizeof _volume::voice);
      dfile.read(reinterpret_cast<char*>(&_volume::ambiance), sizeof _volume::ambiance);
      dfile.read(reinterpret_cast<char*>(&controls::key_menu), sizeof controls::key_menu);

      dfile.read(reinterpret_cast<char*>(&controls::p1_key_up), sizeof controls::p1_key_up);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_down), sizeof controls::p1_key_down);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_left), sizeof controls::p1_key_left);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_right), sizeof controls::p1_key_right);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action1), sizeof controls::p1_key_action1);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action2), sizeof controls::p1_key_action2);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action3), sizeof controls::p1_key_action3);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action4), sizeof controls::p1_key_action4);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action5), sizeof controls::p1_key_action5);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action6), sizeof controls::p1_key_action6);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action7), sizeof controls::p1_key_action7);
      dfile.read(reinterpret_cast<char*>(&controls::p1_key_action8), sizeof controls::p1_key_action8);

      dfile.read(reinterpret_cast<char*>(&controls::p1_button_up), sizeof controls::p1_button_up);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_down), sizeof controls::p1_button_down);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_left), sizeof controls::p1_button_left);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_right), sizeof controls::p1_button_right);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action1), sizeof controls::p1_button_action1);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action2), sizeof controls::p1_button_action2);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action3), sizeof controls::p1_button_action3);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action4), sizeof controls::p1_button_action4);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action5), sizeof controls::p1_button_action5);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action6), sizeof controls::p1_button_action6);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action7), sizeof controls::p1_button_action7);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_action8), sizeof controls::p1_button_action8);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_start), sizeof controls::p1_button_start);
      dfile.read(reinterpret_cast<char*>(&controls::p1_button_select), sizeof controls::p1_button_select);

      dfile.read(reinterpret_cast<char*>(&controls::p2_key_up), sizeof controls::p2_key_up);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_down), sizeof controls::p2_key_down);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_left), sizeof controls::p2_key_left);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_right), sizeof controls::p2_key_right);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action1), sizeof controls::p2_key_action1);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action2), sizeof controls::p2_key_action2);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action3), sizeof controls::p2_key_action3);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action4), sizeof controls::p2_key_action4);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action5), sizeof controls::p2_key_action5);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action6), sizeof controls::p2_key_action6);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action7), sizeof controls::p2_key_action7);
      dfile.read(reinterpret_cast<char*>(&controls::p2_key_action8), sizeof controls::p2_key_action8);

      dfile.read(reinterpret_cast<char*>(&controls::p2_button_up), sizeof controls::p2_button_up);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_down), sizeof controls::p2_button_down);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_left), sizeof controls::p2_button_left);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_right), sizeof controls::p2_button_right);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action1), sizeof controls::p2_button_action1);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action2), sizeof controls::p2_button_action2);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action3), sizeof controls::p2_button_action3);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action4), sizeof controls::p2_button_action4);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action5), sizeof controls::p2_button_action5);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action6), sizeof controls::p2_button_action6);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action7), sizeof controls::p2_button_action7);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_action8), sizeof controls::p2_button_action8);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_start), sizeof controls::p2_button_start);
      dfile.read(reinterpret_cast<char*>(&controls::p2_button_select), sizeof controls::p2_button_select);
    } catch(...) {
      dfile.close();
      return false;
    }

    dfile.close();
    return true;
  };

  /*!
   * \brief Save engine config settings to file.
   * \return False on fail, true on success.
   */
  static bool save(void) {
    std::ofstream dfile("settings.cfg", std::ios::binary | std::ofstream::trunc);
    if(!dfile.good()) return false;

    try {
      dfile.write(reinterpret_cast<const char*>(&flags::draw_fps), sizeof flags::draw_fps);
      dfile.write(reinterpret_cast<const char*>(&gfx::vsync), sizeof gfx::vsync);
      dfile.write(reinterpret_cast<const char*>(&gfx::display_mode), sizeof gfx::display_mode);
      dfile.write(reinterpret_cast<const char*>(&gfx::scale_factor), sizeof gfx::scale_factor);
      dfile.write(reinterpret_cast<const char*>(&gfx::screen_w), sizeof gfx::screen_w);
      dfile.write(reinterpret_cast<const char*>(&gfx::screen_h), sizeof gfx::screen_h);
      dfile.write(reinterpret_cast<const char*>(&volume::main), sizeof volume::main);
      dfile.write(reinterpret_cast<const char*>(&volume::music), sizeof volume::music);
      dfile.write(reinterpret_cast<const char*>(&volume::sample), sizeof volume::sample);
      dfile.write(reinterpret_cast<const char*>(&volume::voice), sizeof volume::voice);
      dfile.write(reinterpret_cast<const char*>(&volume::ambiance), sizeof volume::ambiance);
      dfile.write(reinterpret_cast<const char*>(&controls::key_menu), sizeof controls::key_menu);
      
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_up), sizeof controls::p1_key_up);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_down), sizeof controls::p1_key_down);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_left), sizeof controls::p1_key_left);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_right), sizeof controls::p1_key_right);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action1), sizeof controls::p1_key_action1);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action2), sizeof controls::p1_key_action2);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action3), sizeof controls::p1_key_action3);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action4), sizeof controls::p1_key_action4);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action5), sizeof controls::p1_key_action5);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action6), sizeof controls::p1_key_action6);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action7), sizeof controls::p1_key_action7);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_key_action8), sizeof controls::p1_key_action8);
      
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_up), sizeof controls::p1_button_up);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_down), sizeof controls::p1_button_down);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_left), sizeof controls::p1_button_left);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_right), sizeof controls::p1_button_right);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action1), sizeof controls::p1_button_action1);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action2), sizeof controls::p1_button_action2);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action3), sizeof controls::p1_button_action3);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action4), sizeof controls::p1_button_action4);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action5), sizeof controls::p1_button_action5);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action6), sizeof controls::p1_button_action6);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action7), sizeof controls::p1_button_action7);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_action8), sizeof controls::p1_button_action8);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_start), sizeof controls::p1_button_start);
      dfile.write(reinterpret_cast<const char*>(&controls::p1_button_select), sizeof controls::p1_button_select);
      
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_up), sizeof controls::p2_key_up);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_down), sizeof controls::p2_key_down);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_left), sizeof controls::p2_key_left);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_right), sizeof controls::p2_key_right);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action1), sizeof controls::p2_key_action1);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action2), sizeof controls::p2_key_action2);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action3), sizeof controls::p2_key_action3);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action4), sizeof controls::p2_key_action4);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action5), sizeof controls::p2_key_action5);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action6), sizeof controls::p2_key_action6);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action7), sizeof controls::p2_key_action7);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_key_action8), sizeof controls::p2_key_action8);

      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_up), sizeof controls::p2_button_up);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_down), sizeof controls::p2_button_down);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_left), sizeof controls::p2_button_left);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_right), sizeof controls::p2_button_right);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action1), sizeof controls::p2_button_action1);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action2), sizeof controls::p2_button_action2);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action3), sizeof controls::p2_button_action3);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action4), sizeof controls::p2_button_action4);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action5), sizeof controls::p2_button_action5);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action6), sizeof controls::p2_button_action6);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action7), sizeof controls::p2_button_action7);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_action8), sizeof controls::p2_button_action8);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_start), sizeof controls::p2_button_start);
      dfile.write(reinterpret_cast<const char*>(&controls::p2_button_select), sizeof controls::p2_button_select);
    } catch(...) {
      dfile.close();
      return false;
    }

    dfile.close();
    return true;
  };

  protected:
    config() {
      if(initialized == true)
        throw engine_error("Config instance already running!");
      initialized = true;
    };

  private:
    struct _flags {
      inline static bool is_running = true;
      inline static bool engine_started = false;
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

    static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
