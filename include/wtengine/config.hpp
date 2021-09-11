/*!
 * WTEngine | File:  config.hpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_CONFIG_HPP
#define WTE_CONFIG_HPP

#include <allegro5/allegro.h>

#include <fstream>

#include "wtengine/_globals/_defines.hpp"

namespace wte::mgr {
    class audio;
    class menus;
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
    friend class mgr::menus;
    friend class mgr::renderer;

    public:
        virtual ~config() = default;                  //!<  Default virtual destructor.
        config(const config&) = delete;               //!<  Delete copy constructor.
        void operator=(config const&) = delete;       //!<  Delete assignment operator.

        /*!
         * \struct flags
         * \brief Engine flags.
         */
        struct flags {
            static const bool& is_running;            //!<  Flag to check if the game is running.
            static const bool& game_started;          //!<  Flag to check if the game has started.
            static const bool& menu_opened;           //!<  Flag to check if the menu is opened.
            static const bool& record_input;          //!<  Flag to enable/disable input recording.
            #if WTE_DEBUG_MODE
            static const bool& show_hitboxes;         //!<  Flag to enable/disable hitbox rendering.
            #endif
            inline static bool draw_fps = true;       //!<  Flag to check if fps should be drawn.
            inline static bool input_enabled = true;  //!<  Flag to check if input is enabled.
        };

        /*!
         * \struct volume
         * \brief Volume levels.
         */
        struct volume {
            static const float& main;                 //!<  Main volume level.
            static const float& mix1;                 //!<  Mix 1 volume level.
            static const float& mix2;                 //!<  Mix 2 volume level.
            static const float& mix3;                 //!<  Mix 3 volume level.
            static const float& mix4;                 //!<  Mix 4 volume level.
        };

        /*!
         * \struct gfx
         * \brief Graphics settings.
         */
        struct gfx {
            static const int& screen_w;               //!<  Screen width.
            static const int& screen_h;               //!<  Screen height.
            static const int& arena_w;                //!<  Arena width.
            static const int& arena_h;                //!<  Arena height.
            static const std::size_t& vsync;          //!<  Vsync setting.
            static const std::size_t& display_mode;   //!<  Display mode setting.
            static const float& scale_factor;         //!<  Arena scale factor.
            static const float& menu_scale_factor;    //!<  Menu scale factor.
        };

        /*!
         * \struct controls
         * \brief Control binding settings.
         */
        struct controls {
            static const int& key_menu;               //!<  Open menu key.
            static const int& key_menu_select;        //!<  Select menu item key.

            /*
             * Player 1 keyboard
             */
            static const int& p1_key_up;              //!<  Player 1 input up key.
            static const int& p1_key_down;            //!<  Player 1 input down key.
            static const int& p1_key_left;            //!<  Player 1 input left key.
            static const int& p1_key_right;           //!<  Player 1 input right key.
            static const int& p1_key_action1;         //!<  Player 1 input action 1 key.
            static const int& p1_key_action2;         //!<  Player 1 input action 2 key.
            static const int& p1_key_action3;         //!<  Player 1 input action 3 key.
            static const int& p1_key_action4;         //!<  Player 1 input action 4 key.
            static const int& p1_key_action5;         //!<  Player 1 input action 5 key.
            static const int& p1_key_action6;         //!<  Player 1 input action 6 key.
            static const int& p1_key_action7;         //!<  Player 1 input action 7 key.
            static const int& p1_key_action8;         //!<  Player 1 input action 8 key.

            /*
             * Player 1 joystick
             */
            static const int& p1_button_up;           //!<  Player 1 input button up.
            static const int& p1_button_down;         //!<  Player 1 input button down.
            static const int& p1_button_left;         //!<  Player 1 input button left.
            static const int& p1_button_right;        //!<  Player 1 input button right.
            static const int& p1_button_action1;      //!<  Player 1 input button action 1.
            static const int& p1_button_action2;      //!<  Player 1 input button action 2.
            static const int& p1_button_action3;      //!<  Player 1 input button action 3.
            static const int& p1_button_action4;      //!<  Player 1 input button action 4.
            static const int& p1_button_action5;      //!<  Player 1 input button action 5.
            static const int& p1_button_action6;      //!<  Player 1 input button action 6.
            static const int& p1_button_action7;      //!<  Player 1 input button action 7.
            static const int& p1_button_action8;      //!<  Player 1 input button action 8.
            static const int& p1_button_start;        //!<  Player 1 input button start.
            static const int& p1_button_select;       //!<  Player 1 input button select.
            static const float& p1_pola_x;            //!<  Player 1 joystick A position X.
            static const float& p1_pola_y;            //!<  Player 1 joystick A position Y.
            static const float& p1_polb_x;            //!<  Player 1 joystick B position X.
            static const float& p1_polb_y;            //!<  Player 1 joystick B position Y.
            static const float& p1_polc_x;            //!<  Player 1 joystick C position X.
            static const float& p1_polc_y;            //!<  Player 1 joystick C position Y.
            static const float& p1_throttle_left;     //!<  Player 1 throttle left.
            static const float& p1_throttle_right;    //!<  Player 1 throttle right.

            /*
             * Player 2 keyboard
             */
            static const int& p2_key_up;              //!<  Player 2 input up key.
            static const int& p2_key_down;            //!<  Player 2 input down key.
            static const int& p2_key_left;            //!<  Player 2 input left key.
            static const int& p2_key_right;           //!<  Player 2 input right key.
            static const int& p2_key_action1;         //!<  Player 2 input action 1 key.
            static const int& p2_key_action2;         //!<  Player 2 input action 2 key.
            static const int& p2_key_action3;         //!<  Player 2 input action 3 key.
            static const int& p2_key_action4;         //!<  Player 2 input action 4 key.
            static const int& p2_key_action5;         //!<  Player 2 input action 5 key.
            static const int& p2_key_action6;         //!<  Player 2 input action 6 key.
            static const int& p2_key_action7;         //!<  Player 2 input action 7 key.
            static const int& p2_key_action8;         //!<  Player 2 input action 8 key.

            /*
             * Player 2 joystick
             */
            static const int& p2_button_up;           //!<  Player 2 input button up.
            static const int& p2_button_down;         //!<  Player 2 input button down.
            static const int& p2_button_left;         //!<  Player 2 input button left.
            static const int& p2_button_right;        //!<  Player 2 input button right.
            static const int& p2_button_action1;      //!<  Player 2 input button action 1.
            static const int& p2_button_action2;      //!<  Player 2 input button action 2.
            static const int& p2_button_action3;      //!<  Player 2 input button action 3.
            static const int& p2_button_action4;      //!<  Player 2 input button action 4.
            static const int& p2_button_action5;      //!<  Player 2 input button action 5.
            static const int& p2_button_action6;      //!<  Player 2 input button action 6.
            static const int& p2_button_action7;      //!<  Player 2 input button action 7.
            static const int& p2_button_action8;      //!<  Player 2 input button action 8.
            static const int& p2_button_start;        //!<  Player 2 input button start.
            static const int& p2_button_select;       //!<  Player 2 input button select.
            static const float& p2_pola_x;            //!<  Player 2 joystick A position X.
            static const float& p2_pola_y;            //!<  Player 2 joystick A position Y.
            static const float& p2_polb_x;            //!<  Player 2 joystick B position X.
            static const float& p2_polb_y;            //!<  Player 2 joystick B position Y.
            static const float& p2_polc_x;            //!<  Player 2 joystick C position X.
            static const float& p2_polc_y;            //!<  Player 2 joystick C position Y.
            static const float& p2_throttle_left;     //!<  Player 2 throttle left.
            static const float& p2_throttle_right;    //!<  Player 2 throttle right.

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

    protected:
        config();

    private:
        struct _flags {
            inline static bool is_running = true;
            inline static bool game_started = false;
            inline static bool menu_opened = true;
            inline static bool record_input = false;
            #if WTE_DEBUG_MODE
            inline static bool show_hitboxes = false;
            #endif
        };

        struct _volume {
            inline static float main = 1.0f;
            inline static float mix1 = 1.0f;
            inline static float mix2 = 1.0f;
            inline static float mix3 = 1.0f;
            inline static float mix4 = 1.0f;
        };

        struct _gfx {
            inline static int screen_w = 0, screen_h = 0;
            inline static int arena_w = 0, arena_h = 0;
            inline static std::size_t vsync = 2;
            inline static std::size_t display_mode = 0;
            inline static float scale_factor = 1.0f;
            inline static float menu_scale_factor = 1.0f;
        };

        struct _controls {
            inline static int key_menu = WTE_KEY_MENU_DEFAULT;
            inline static int key_menu_select = WTE_KEY_MENU_SELECT_DEFAULT;

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
            inline static int p1_key_action7 = WTE_P1_KEY_ACTION7_DEFAULT;
            inline static int p1_key_action8 = WTE_P1_KEY_ACTION8_DEFAULT;

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
            inline static int p2_key_action7 = WTE_P2_KEY_ACTION7_DEFAULT;
            inline static int p2_key_action8 = WTE_P2_KEY_ACTION8_DEFAULT;

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

        static bool initialized;  //  Restrict to one instance.
};

}  //  end namespace wte

#endif
