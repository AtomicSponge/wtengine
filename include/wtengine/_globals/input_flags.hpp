/*!
 * WTEngine | File:  input_flags.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_INPUT_FLAGS_HPP
#define WTE_INPUT_FLAGS_HPP

#include <cassert>

#include "wtengine/wte_global_defines.hpp"

namespace wte
{

/*!
 * \enum WTE_JOYSTICK_FLAGS
 * Joystick flags.
 */
enum WTE_JOYSTICK_FLAGS {
    WTE_JOYSTICK_A, WTE_JOYSTICK_B,
    LEFT_TRIGGER, RIGHT_TRIGGER,
    WTE_MAX_JOYSTICK_FLAGS
};

/*!
 * \enum WTE_DIRECTION_FLAG_ENUM
 * Directional flags.
 */
enum WTE_DIRECTION_FLAG_ENUM {
    WTE_INPUT_DIRECTION_SET,     WTE_INPUT_DIRECTION_UNSET,
    WTE_MAX_DIRECTON_FLAGS
};

/*!
 * \enum WTE_INPUT_BUTTON_FLAG_ENUM
 * Input button flags.
 */
enum WTE_INPUT_BUTTON_FLAG_ENUM {
    WTE_INPUT_BUTTON_UP,        WTE_INPUT_BUTTON_DOWN,
    WTE_INPUT_BUTTON_LEFT,      WTE_INPUT_BUTTON_RIGHT,

    WTE_INPUT_BUTTON_1,         WTE_INPUT_BUTTON_2,
    WTE_INPUT_BUTTON_3,         WTE_INPUT_BUTTON_4,
    WTE_INPUT_BUTTON_5,         WTE_INPUT_BUTTON_6,
    WTE_INPUT_BUTTON_7,         WTE_INPUT_BUTTON_8,

    #if WTE_INPUT_MODE == 0
    //  Add the trigger buttons if in digital gamepad mode.
    WTE_INPUT_BUTTON_LEFT_TRIGGER,
    WTE_INPUT_BUTTON_RIGHT_TRIGGER,
    #endif

    WTE_INPUT_BUTTON_START,     WTE_INPUT_BUTTON_SELECT,

    WTE_INPUT_MENU_SELECT,
    WTE_INPUT_MENU_CLOSE,

    WTE_MAX_INPUT_BUTTON_FLAGS
};

/*!
 * \enum WTE_BUTTON_EVENT_FLAGS
 * Button event flags.
 */
enum WTE_BUTTON_EVENT_FLAGS {
    WTE_BUTTON_EVENT_DOWN,
    WTE_BUTTON_EVENT_UP,
    WTE_MAX_BUTTON_EVENT_FLAGS
};

/*!
 * \class input_flags
 * \brief Colletion of flags to store input state.
 */
class input_flags final {
    public:
        //!  Remove copy constructor
        input_flags(const input_flags&) = delete;
        //!  Remove assignment operator
        void operator=(input_flags const&) = delete;

        /*!
         * \brief Set all input flags to false or zero.
         */
        static void unset_all(void);

        /*!
         * \brief Toggle flag to set.
         * 
         * \param j Joystick to set flag for.
         * \param d Direction flag to set.
         */
        static void joystick_toggle(const std::size_t& j, const std::size_t& d);

        /*!
         * \brief Check if a flag is set.
         * 
         * Unsets the flag if it is.
         * 
         * \param j Joystick to check.
         * \param d Direction flag to check.
         * \return True if set, false if not.
         */
        static const bool joystick_check(const std::size_t& j, const std::size_t& d);

        /*!
         * \brief Set radians for a joystick.
         * 
         * \param j Joystick to set for.
         * \param a Angle in radians.
         */
        static void set_joystick_radians(const std::size_t& j, const float& a);

        /*!
         * \brief Set the direction of a joystick.
         * 
         * \param j Joystick to check.
         * \return Angle in radians.
         */
        static const float get_joystick_radians(const std::size_t& j);

        /*!
         * \brief Set X pole of a joystick.
         * 
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        static void set_joystick_pol_x(const std::size_t& j, const float& d);

        /*!
         * \brief Set Y pole of a joystick.
         * 
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        static void set_joystick_pol_y(const std::size_t& j, const float& d);

        /*!
         * \brief Get X pole for a joystick.
         * 
         * \param j Joystick to set.
         * \return Joystick X direction.
         */
        static const float get_joystick_pol_x(const std::size_t& j);

        /*!
         * \brief Get Y pole for a joystick.
         * 
         * \param j Joystick to set.
         * \return Joystick Y direction.
         */
        static const float get_joystick_pol_y(const std::size_t& j);

        /*!
         * \brief Check if a button event is set.
         * 
         * \param b Button to check.
         * \param e Event to check.
         */
        static const bool check_button_event(const std::size_t& b, const std::size_t& e);

        /*!
         * \brief Set a button event.
         * 
         * \param b Button to set.
         * \param e Event to set.
         */
        static void set_button_event(const std::size_t& b, const std::size_t& e);

        /*!
         * \brief Get the last key that was pressed.
         * 
         * \return Key code.
         */
        static const std::size_t get_last_keypress(void);

        /*!
         * \brief Set the last key pressed.
         * 
         * \param lk Key code of last key pressed.
         */
        static void set_last_keypress(const std::size_t& lk);

        /*!
         * \brief Get the last button that was pressed.
         * 
         * \return Button code.
         */
        static const std::size_t get_last_buttonpress(void);

        /*!
         * \brief Set the last button pressed.
         * 
         * \param lk Code of last button pressed.
         */
        static void set_last_buttonpress(const std::size_t& lk);

    private:
        inline input_flags() { unset_all(); };
        inline ~input_flags() { unset_all(); };

        inline static std::size_t last_keypress = 0;
        inline static std::size_t last_buttonpress = 0;

        static bool dflags[WTE_MAX_JOYSTICK_FLAGS][WTE_MAX_DIRECTON_FLAGS];

        inline static float angle[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static float pol_x[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static float pol_y[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };

        static bool buttons[WTE_MAX_INPUT_BUTTON_FLAGS][WTE_MAX_BUTTON_EVENT_FLAGS];
};

} //  end namespace wte

#endif
