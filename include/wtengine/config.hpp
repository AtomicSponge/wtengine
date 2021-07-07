/*!
 * WTEngine | File:  config.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CONFIG_HPP
#define WTE_CONFIG_HPP

#include <allegro5/allegro.h>

#include <string>
#include <fstream>

namespace wte
{

/*!
 * \class config
 * \brief Store engine config variables in a globally accessible class.
 */
class config {
    public:
        inline virtual ~config() {};

        struct flags {
            inline static bool is_running = true;
            inline static bool game_started = false;
            inline static bool game_menu_opened = true;
            inline static bool draw_fps = false;
        };

        struct volume {
            inline static float main = 1.0f;
            inline static float mix1 = 1.0f;
            inline static float mix2 = 1.0f;
            inline static float mix3 = 1.0f;
            inline static float mix4 = 1.0f;
        };

        struct controls {
            /*
            * Player 1 keyboard
            */
            inline static int p1_key_up = 0;
            inline static int p1_key_down = 0;
            inline static int p1_key_left = 0;
            inline static int p1_key_right = 0;
            inline static int p1_key_action1 = ALLEGRO_KEY_LCTRL;
            inline static int p1_key_action2 = ALLEGRO_KEY_ALT;
            inline static int p1_key_action3 = 0;
            inline static int p1_key_action4 = 0;
            inline static int p1_key_action5 = 0;
            inline static int p1_key_action6 = 0;

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
            inline static float p1_pola_x = 0.0f;
            inline static float p1_pola_y = 0.0f;
            inline static float p1_polb_x = 0.0f;
            inline static float p1_polb_y = 0.0f;
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
            inline static float p2_pola_x = 0.0f;
            inline static float p2_pola_y = 0.0f;
            inline static float p2_polb_x = 0.0f;
            inline static float p2_polb_y = 0.0f;
            inline static float p2_throttle_left = 0.0f;
            inline static float p2_throttle_right = 0.0f;
        };

        struct gfx {
            inline static std::size_t vsync = 0;
            inline static std::size_t display_mode = 0;
            inline static float scale_factor = 1.0f;
        };

        /*!
         * \brief Load engine config settings from file.
         * 
         * Will create the config variable if it does not exist.
         * If it does exist, the value is updated from the file.
         * 
         * \return False on fail, true on success.
         */
        static bool load(void);

        /*!
         * \brief Save engine config settings to file.
         * 
         * \return False on fail, true on success.
         */
        static bool save(void);

    protected:
        inline config() {};
};

} //  end namespace wte

#endif
