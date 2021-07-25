/*!
 * WTEngine | File:  config.hpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CONFIG_HPP
#define WTE_CONFIG_HPP

#include <allegro5/allegro.h>

#include <fstream>

#include "wtengine/_globals/_defines.hpp"

namespace wte
{

/*!
 * \class config
 * \brief Store engine config variables in a globally accessible class.
 */
class config {
    public:
        inline virtual ~config() {};

        /*!
         * \struct flags
         * \brief Engine flags.
         */
        struct flags {
            inline static bool is_running = true;
            inline static bool game_started = false;
            inline static bool game_menu_opened = true;
            inline static bool draw_fps = true;
            inline static bool record_input = false;
            inline static bool input_enabled = true;
        };

        /*!
         * \struct volume
         * \brief Volume levels.
         */
        struct volume {
            inline static float main = 1.0f;
            inline static float mix1 = 1.0f;
            inline static float mix2 = 1.0f;
            inline static float mix3 = 1.0f;
            inline static float mix4 = 1.0f;
        };

        /*!
         * \struct gfx
         * \brief Graphics settings.
         */
        struct gfx {
            inline static std::size_t vsync = 2;
            inline static std::size_t display_mode = 0;
            inline static float scale_factor = 1.0f;
        };

        /*!
         * \struct controls
         * \brief Control binding settings.
         */
        struct controls {
            inline static int key_menu = WTE_KEY_MENU_DEFAULT;
            inline static int key_menu_select = WTE_KET_MENU_SELECT_DEFAULT;

            /*
             * Player 1 keyboard
             */
            inline static int p1_key_up = WTE_P1_KEY_UP_DEFAULT;
            inline static int p1_key_down = WTE_P1_KEY_DOWN_DEFAULT;
            inline static int p1_key_left = WTE_P1_KEY_LEFT_DEFAULT;
            inline static int p1_key_right = WTE_P1_KEY_RIGHT_DEFAULT;
            inline static int p1_key_action1 = WTE_P1_KEY_ACTION1_DEFAULT;
            inline static int p1_key_action2 = WTE_P1_KEY_ACTION2_DEFAULT;
            inline static int p1_key_action3 = WTE_P1_KEY_ACTION3_DEFAULT;
            inline static int p1_key_action4 = WTE_P1_KEY_ACTION4_DEFAULT;
            inline static int p1_key_action5 = WTE_P1_KEY_ACTION5_DEFAULT;
            inline static int p1_key_action6 = WTE_P1_KEY_ACTION6_DEFAULT;

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
            inline static int p2_key_up = WTE_P2_KEY_UP_DEFAULT;
            inline static int p2_key_down = WTE_P2_KEY_DOWN_DEFAULT;
            inline static int p2_key_left = WTE_P2_KEY_LEFT_DEFAULT;
            inline static int p2_key_right = WTE_P2_KEY_RIGHT_DEFAULT;
            inline static int p2_key_action1 = WTE_P2_KEY_ACTION1_DEFAULT;
            inline static int p2_key_action2 = WTE_P2_KEY_ACTION2_DEFAULT;
            inline static int p2_key_action3 = WTE_P2_KEY_ACTION3_DEFAULT;
            inline static int p2_key_action4 = WTE_P2_KEY_ACTION4_DEFAULT;
            inline static int p2_key_action5 = WTE_P2_KEY_ACTION5_DEFAULT;
            inline static int p2_key_action6 = WTE_P2_KEY_ACTION6_DEFAULT;

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
        static const bool load(void);

        /*!
         * \brief Save engine config settings to file.
         * \return False on fail, true on success.
         */
        static const bool save(void);

    protected:
        inline config() {};
};

} //  end namespace wte

#endif
