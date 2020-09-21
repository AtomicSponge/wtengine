/*!
 * WTEngine | File:  wte_input_map.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_INPUT_MAP_HPP
#define WTE_INPUT_MAP_HPP

#include <allegro5/allegro.h>

#include "_globals/engine_cfg.hpp"
#include "_globals/input_flags.hpp"

namespace wte
{

/*!
 * \class wte_input_map
 * \brief Map buttons and keyboard
 */
class wte_input_map {
    public:
        /*!
         * \brief Input Map destructor.
         */
        inline virtual ~wte_input_map() {};

    protected:
        /*!
         * \brief Input Map constructor.
         */
        inline wte_input_map() {
            for(std::size_t i = 0; i < WTE_MAX_INPUT_BUTTON_FLAGS; i++) {
                for(std::size_t j = 0; j < MAX_KEY_SETS; j++) {
                    keyboard_bind[i][j] = 0;
                }
                button_bind[i] = 0;
            }
        };

        /*!
         * \brief Set the keyboard and gamepad button binding arrays from the engine cfg settings.
         * 
         * \param joy_installed Is a gamepad installed.
         */
        inline void set_binding(const bool& joy_installed) {
            check_reg(joy_installed);  //  Make sure all settings are registered.

            //  Keyboard Set A
            keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_up");
            keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_down");
            keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_left");
            keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_right");
            keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_action_1");
            keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_action_2");
            keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_action_3");
            keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_A] = engine_cfg::get<std::size_t>("set_a_key_action_4");

            //  Keyboard Set B
            keyboard_bind[WTE_INPUT_BUTTON_UP][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_up");
            keyboard_bind[WTE_INPUT_BUTTON_DOWN][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_down");
            keyboard_bind[WTE_INPUT_BUTTON_LEFT][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_left");
            keyboard_bind[WTE_INPUT_BUTTON_RIGHT][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_right");
            keyboard_bind[WTE_INPUT_BUTTON_1][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_action_1");
            keyboard_bind[WTE_INPUT_BUTTON_2][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_action_2");
            keyboard_bind[WTE_INPUT_BUTTON_3][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_action_3");
            keyboard_bind[WTE_INPUT_BUTTON_4][KEY_SET_B] = engine_cfg::get<std::size_t>("set_b_key_action_4");

            if(joy_installed) {
                //  Joystick buttons
                button_bind[WTE_INPUT_BUTTON_UP] = engine_cfg::get<std::size_t>("joy_up");
                button_bind[WTE_INPUT_BUTTON_DOWN] = engine_cfg::get<std::size_t>("joy_down");
                button_bind[WTE_INPUT_BUTTON_LEFT] = engine_cfg::get<std::size_t>("joy_left");
                button_bind[WTE_INPUT_BUTTON_RIGHT] = engine_cfg::get<std::size_t>("joy_right");
                button_bind[WTE_INPUT_BUTTON_1] = engine_cfg::get<std::size_t>("joy_action_1");
                button_bind[WTE_INPUT_BUTTON_2] = engine_cfg::get<std::size_t>("joy_action_2");
                button_bind[WTE_INPUT_BUTTON_3] = engine_cfg::get<std::size_t>("joy_action_3");
                button_bind[WTE_INPUT_BUTTON_4] = engine_cfg::get<std::size_t>("joy_action_4");
                button_bind[WTE_INPUT_BUTTON_5] = engine_cfg::get<std::size_t>("joy_action_5");
                button_bind[WTE_INPUT_BUTTON_6] = engine_cfg::get<std::size_t>("joy_action_6");
                button_bind[WTE_INPUT_BUTTON_7] = engine_cfg::get<std::size_t>("joy_action_7");
                button_bind[WTE_INPUT_BUTTON_8] = engine_cfg::get<std::size_t>("joy_action_8");
                button_bind[WTE_INPUT_BUTTON_START] = engine_cfg::get<std::size_t>("joy_start");
                button_bind[WTE_INPUT_BUTTON_SELECT] = engine_cfg::get<std::size_t>("joy_select");
            }
        };

        /*!
         * \enum WTE_KEY_SET_ENUM
         * List of key sets.
         */
        enum WTE_KEY_SET_ENUM {
            KEY_SET_A, KEY_SET_B, MAX_KEY_SETS
        };

        //!  Store the keyboard binding.
        int keyboard_bind[WTE_MAX_INPUT_BUTTON_FLAGS][MAX_KEY_SETS];
        //!  Store the button binding.
        int button_bind[WTE_MAX_INPUT_BUTTON_FLAGS];

    private:
        /*!
         * \brief Verify all necessary engine cfg settings are registered.
         * 
         * If a binding setting isn't registered, create it and set a default value.
         * 
         * \param joy_installed Is a gamepad installed.
         */
        inline void check_reg(const bool& joy_installed) {
            //  Keyboard Set A
            if(!engine_cfg::is_reg("set_a_key_up")) engine_cfg::reg("set_a_key_up", std::to_string(ALLEGRO_KEY_UP));
            if(!engine_cfg::is_reg("set_a_key_down")) engine_cfg::reg("set_a_key_down", std::to_string(ALLEGRO_KEY_DOWN));
            if(!engine_cfg::is_reg("set_a_key_left")) engine_cfg::reg("set_a_key_left", std::to_string(ALLEGRO_KEY_LEFT));
            if(!engine_cfg::is_reg("set_a_key_right")) engine_cfg::reg("set_a_key_right", std::to_string(ALLEGRO_KEY_RIGHT));
            if(!engine_cfg::is_reg("set_a_key_action_1")) engine_cfg::reg("set_a_key_action_1", std::to_string(ALLEGRO_KEY_LCTRL));
            if(!engine_cfg::is_reg("set_a_key_action_2")) engine_cfg::reg("set_a_key_action_2", std::to_string(ALLEGRO_KEY_ALT));
            if(!engine_cfg::is_reg("set_a_key_action_3")) engine_cfg::reg("set_a_key_action_3", std::to_string(ALLEGRO_KEY_LSHIFT));
            if(!engine_cfg::is_reg("set_a_key_action_4")) engine_cfg::reg("set_a_key_action_4", std::to_string(ALLEGRO_KEY_SPACE));

            //  Keyboard Set B
            if(!engine_cfg::is_reg("set_b_key_up")) engine_cfg::reg("set_b_key_up", std::to_string(ALLEGRO_KEY_W));
            if(!engine_cfg::is_reg("set_b_key_down")) engine_cfg::reg("set_b_key_down", std::to_string(ALLEGRO_KEY_S));
            if(!engine_cfg::is_reg("set_b_key_left")) engine_cfg::reg("set_b_key_left", std::to_string(ALLEGRO_KEY_A));
            if(!engine_cfg::is_reg("set_b_key_right")) engine_cfg::reg("set_b_key_right", std::to_string(ALLEGRO_KEY_D));
            if(!engine_cfg::is_reg("set_b_key_action_1")) engine_cfg::reg("set_b_key_action_1", std::to_string(ALLEGRO_KEY_RCTRL));
            if(!engine_cfg::is_reg("set_b_key_action_2")) engine_cfg::reg("set_b_key_action_2", std::to_string(ALLEGRO_KEY_ALTGR));
            if(!engine_cfg::is_reg("set_b_key_action_3")) engine_cfg::reg("set_b_key_action_3", std::to_string(ALLEGRO_KEY_RSHIFT));
            if(!engine_cfg::is_reg("set_b_key_action_4")) engine_cfg::reg("set_b_key_action_4", std::to_string(ALLEGRO_KEY_SPACE));

            if(joy_installed) {
                //  Joystick buttons
                if(!engine_cfg::is_reg("joy_up")) engine_cfg::reg("joy_up", std::to_string(13));
                if(!engine_cfg::is_reg("joy_down")) engine_cfg::reg("joy_down", std::to_string(12));
                if(!engine_cfg::is_reg("joy_left")) engine_cfg::reg("joy_left", std::to_string(11));
                if(!engine_cfg::is_reg("joy_right")) engine_cfg::reg("joy_right", std::to_string(10));
                if(!engine_cfg::is_reg("joy_action_1")) engine_cfg::reg("joy_action_1", std::to_string(1));
                if(!engine_cfg::is_reg("joy_action_2")) engine_cfg::reg("joy_action_2", std::to_string(0));
                if(!engine_cfg::is_reg("joy_action_3")) engine_cfg::reg("joy_action_3", std::to_string(3));
                if(!engine_cfg::is_reg("joy_action_4")) engine_cfg::reg("joy_action_4", std::to_string(2));
                if(!engine_cfg::is_reg("joy_action_5")) engine_cfg::reg("joy_action_5", std::to_string(4));
                if(!engine_cfg::is_reg("joy_action_6")) engine_cfg::reg("joy_action_6", std::to_string(5));
                if(!engine_cfg::is_reg("joy_action_7")) engine_cfg::reg("joy_action_7", std::to_string(6));
                if(!engine_cfg::is_reg("joy_action_8")) engine_cfg::reg("joy_action_8", std::to_string(7));
                if(!engine_cfg::is_reg("joy_start")) engine_cfg::reg("joy_start", std::to_string(9));
                if(!engine_cfg::is_reg("joy_select")) engine_cfg::reg("joy_select", std::to_string(8));
            }
        };
};

} //  end namespace wte

#endif
