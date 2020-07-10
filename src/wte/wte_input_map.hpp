/*!
 * \brief WTEngine | File:  wte_input_map.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Map input.
 * 
 * \bug WIP
 */

#ifndef WTE_INPUT_MAP_HPP
#define WTE_INPUT_MAP_HPP

#include <allegro5/allegro.h>

#include "_globals/engine_cfg.hpp"
#include "_globals/input_flags.hpp"

namespace wte
{

/*!
 * \class wte_input_map class
 * Map buttons and keyboard
 */
class wte_input_map {
    public:
        /*!
         *
         */
        inline virtual ~wte_input_map() {};

    protected:
        /*!
         *
         */
        inline wte_input_map() {};

        /*!
         *
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
                button_bind[WTE_INPUT_MENU_CLOSE] = engine_cfg::get<std::size_t>("joy_menu");
            }
        };

        /*!
         * \enum List of key sets.
         */
        enum WTE_KEY_SET_ENUM {
            KEY_SET_A, KEY_SET_B, MAX_KEY_SETS
        };

        std::size_t keyboard_bind[WTE_MAX_INPUT_BUTTON_FLAGS][MAX_KEY_SETS];
        std::size_t button_bind[WTE_MAX_INPUT_BUTTON_FLAGS];

    private:
        /*!
         *
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
                if(!engine_cfg::is_reg("joy_action_1")) engine_cfg::reg("joy_action_1", std::to_string(0));
                if(!engine_cfg::is_reg("joy_action_2")) engine_cfg::reg("joy_action_2", std::to_string(1));
                if(!engine_cfg::is_reg("joy_action_3")) engine_cfg::reg("joy_action_3", std::to_string(2));
                if(!engine_cfg::is_reg("joy_action_4")) engine_cfg::reg("joy_action_4", std::to_string(3));
                if(!engine_cfg::is_reg("joy_menu")) engine_cfg::reg("joy_menu", std::to_string(9));
            }
        };
};

} //  end namespace wte

#endif
