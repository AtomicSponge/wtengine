/*!
 * WTEngine | File:  config.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CONFIG_HPP
#define WTE_CONFIG_HPP

#include <allegro5/allegro.h>

#include <fstream>

#include "wtengine/_globals/_defines.hpp"

namespace wte {

namespace mgr {
    class menus;
    class renderer;
}

/*!
 * \class config
 * \brief Store engine config variables in a globally accessible class.
 */
class config {
    friend class display;
    friend class engine;
    friend class input;
    friend class mgr::menus;
    friend class mgr::renderer;

    public:
        virtual ~config() = default;

        /*!
         * \struct flags
         * \brief Engine flags.
         */
        struct flags {
            static const bool& is_running;
            static const bool& game_started;
            static const bool& game_menu_opened;
            inline static bool draw_fps = true;
            inline static bool record_input = false;
            inline static bool input_enabled = true;
        };

        /*!
         * \struct volume
         * \brief Volume levels.
         */
        struct volume {
            static const float& main;
            static const float& mix1;
            static const float& mix2;
            static const float& mix3;
            static const float& mix4;
        };

        /*!
         * \struct gfx
         * \brief Graphics settings.
         */
        struct gfx {
            static const std::size_t& vsync;
            static const std::size_t& display_mode;
            static const float& scale_factor;
            static const float& menu_scale_factor;
        };

        /*!
         * \struct controls
         * \brief Control binding settings.
         */
        struct controls {
            static const int& key_menu;
            static const int& key_menu_select;

            /*
             * Player 1 keyboard
             */
            static const int& p1_key_up;
            static const int& p1_key_down;
            static const int& p1_key_left;
            static const int& p1_key_right;
            static const int& p1_key_action1;
            static const int& p1_key_action2;
            static const int& p1_key_action3;
            static const int& p1_key_action4;
            static const int& p1_key_action5;
            static const int& p1_key_action6;

            /*
             * Player 1 joystick
             */
            static const int& p1_button_up;
            static const int& p1_button_down;
            static const int& p1_button_left;
            static const int& p1_button_right;
            static const int& p1_button_action1;
            static const int& p1_button_action2;
            static const int& p1_button_action3;
            static const int& p1_button_action4;
            static const int& p1_button_action5;
            static const int& p1_button_action6;
            static const int& p1_button_start;
            static const int& p1_button_select;
            static const float& p1_pola_x;
            static const float& p1_pola_y;
            static const float& p1_polb_x;
            static const float& p1_polb_y;
            static const float& p1_polc_x;
            static const float& p1_polc_y;
            static const float& p1_throttle_left;
            static const float& p1_throttle_right;

            /*
             * Player 2 keyboard
             */
            static const int& p2_key_up;
            static const int& p2_key_down;
            static const int& p2_key_left;
            static const int& p2_key_right;
            static const int& p2_key_action1;
            static const int& p2_key_action2;
            static const int& p2_key_action3;
            static const int& p2_key_action4;
            static const int& p2_key_action5;
            static const int& p2_key_action6;

            /*
             * Player 2 joystick
             */
            static const int& p2_button_up;
            static const int& p2_button_down;
            static const int& p2_button_left;
            static const int& p2_button_right;
            static const int& p2_button_action1;
            static const int& p2_button_action2;
            static const int& p2_button_action3;
            static const int& p2_button_action4;
            static const int& p2_button_action5;
            static const int& p2_button_action6;
            static const int& p2_button_start;
            static const int& p2_button_select;
            static const float& p2_pola_x;
            static const float& p2_pola_y;
            static const float& p2_polb_x;
            static const float& p2_polb_y;
            static const float& p2_polc_x;
            static const float& p2_polc_y;
            static const float& p2_throttle_left;
            static const float& p2_throttle_right;

            /*!
             * \brief Reset P1 joystick polls.
             */
            static void reset_p1_pols(void);

            /*!
             * \brief Reset P2 joystick polls.
             */
            static void reset_p2_pols(void);

            /*!
             * \brief Reset P1 & P2 joystick polls.
             */
            static void reset_pols(void);
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

    private:
        struct _flags {
            inline static bool is_running = true;
            inline static bool game_started = false;
            inline static bool game_menu_opened = true;
        };

        struct _volume {
            inline static float main = 1.0f;
            inline static float mix1 = 1.0f;
            inline static float mix2 = 1.0f;
            inline static float mix3 = 1.0f;
            inline static float mix4 = 1.0f;
        };

        struct _gfx {
            inline static std::size_t vsync = 2;
            inline static std::size_t display_mode = 0;
            inline static float scale_factor = 1.0f;
            inline static float menu_scale_factor = 1.0f;
        };

        struct _controls {
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

    protected:
        config();
        static bool initialized;  //  Restrict to one instance.
};

} //  end namespace wte

#endif
