/*!
 * WTEngine | File:  engine_cfg.hpp
 * 
 * \author Matthew Evans
 * \version 0.3
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_ENGINE_CFG_HPP
#define WTE_ENGINE_CFG_HPP

#include <string>
#include <fstream>

namespace wte
{

/*!
 * \class engine_cfg
 * \brief Store engine config variables in a globally accessible class.
 * 
 * Inherits map functionality from the variable_map class.
 * This class provides specific load/save functions.
 */
class engine_cfg {
    public:
        inline engine_cfg(void) {};

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

        struct input {
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

        class gfx {
            public:
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
        inline static bool load(void) {
            /*std::ifstream data_file("settings.cfg");
            //  Data file doesn't exist, create one
            if(!data_file.good()) {
                save();
                data_file.open("settings.cfg");
            }
            if(!data_file.good()) return false;

            std::string it;
            //  Read each line, try to register or set
            while(std::getline(data_file, it)) {
                if(!reg(it)) set(it);
            }

            data_file.close();*/
            return true;
        };

        /*!
         * \brief Save engine config settings to file.
         * 
         * \return False on fail, true on success.
         */
        inline static bool save(void) {
            /*std::ofstream data_file("settings.cfg");
            if(!data_file.good()) return false;

            //  Write each pair to file
            for(auto it = _map.cbegin(); it != _map.cend(); it++) {
                data_file << it->first << "=" << it->second << std::endl;
            }

            data_file.close();*/
            return true;
        };

    protected:
        inline ~engine_cfg(void) {};
};

} //  end namespace wte

#endif
