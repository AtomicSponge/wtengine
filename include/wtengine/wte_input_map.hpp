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
        virtual ~wte_input_map();

    protected:
        /*!
         * \brief Input Map constructor.
         */
        wte_input_map();

        /*!
         * \brief Set the keyboard and gamepad button binding arrays from the engine cfg settings.
         * 
         * \param joy_installed Is a gamepad installed.
         */
        void set_binding(const bool& joy_installed);

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
        void check_reg(const bool& joy_installed);
};

} //  end namespace wte

#endif
