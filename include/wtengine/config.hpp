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
        virtual ~config() = default;                  //!<  Default virtual destructor.
        config(const config&) = delete;               //!<  Delete copy constructor.
        void operator=(config const&) = delete;       //!<  Delete assignment operator.

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
        static bool load(void);

        /*!
         * \brief Save engine config settings to file.
         * \return False on fail, true on success.
         */
        static bool save(void);

    protected:
        config();

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
