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
        inline static void unset_all(void) {
            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                for(std::size_t d = 0; d < WTE_MAX_DIRECTON_FLAGS; d++)
                    dflags[j][d] = false;

            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                angle[j] = 0.0f;
            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                pol_x[j] = 0.0f;
            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                pol_y[j] = 0.0f;

            for(std::size_t b = 0; b < WTE_MAX_INPUT_BUTTON_FLAGS; b++)
                for(std::size_t e = 0; e < WTE_MAX_BUTTON_EVENT_FLAGS; e++)
                    buttons[b][e] = false;
        };

        /*!
         * \brief Toggle flag to set.
         * 
         * \param j Joystick to set flag for.
         * \param d Direction flag to set.
         */
        inline static void joystick_toggle(const std::size_t& j, const std::size_t& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            assert(d < WTE_MAX_DIRECTON_FLAGS);

            dflags[j][d] = true;
        };

        /*!
         * \brief Check if a flag is set.
         * 
         * Unsets the flag if it is.
         * 
         * \param j Joystick to check.
         * \param d Direction flag to check.
         * \return True if set, false if not.
         */
        inline static const bool joystick_check(const std::size_t& j, const std::size_t& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            assert(d < WTE_MAX_DIRECTON_FLAGS);
            
            const bool consume = dflags[j][d];
            if(consume) dflags[j][d] = false;
            return consume;
        };

        /*!
         * \brief Set radians for a joystick.
         * 
         * \param j Joystick to set for.
         * \param a Angle in radians.
         */
        inline static void set_joystick_radians(const std::size_t& j, const float& a) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            angle[j] = a;
        };

        /*!
         * \brief Set the direction of a joystick.
         * 
         * \param j Joystick to check.
         * \return Angle in radians.
         */
        inline static const float get_joystick_radians(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            return angle[j];
        };

        /*!
         * \brief Set X pole of a joystick.
         * 
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_x(const std::size_t& j, const float& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            pol_x[j] = d;
        };

        /*!
         * \brief Set Y pole of a joystick.
         * 
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_y(const std::size_t& j, const float& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            pol_y[j] = d;
        };

        /*!
         * \brief Get X pole for a joystick.
         * 
         * \param j Joystick to set.
         * \return Joystick X direction.
         */
        inline static const float get_joystick_pol_x(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            return pol_x[j];
        };

        /*!
         * \brief Get Y pole for a joystick.
         * 
         * \param j Joystick to set.
         * \return Joystick Y direction.
         */
        inline static const float get_joystick_pol_y(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);

            return pol_y[j];
        };

        /*!
         * \brief Check if a button event is set.
         * 
         * \param b Button to check.
         * \param e Event to check.
         */
        inline static const bool check_button_event(const std::size_t& b, const std::size_t& e) {
            assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
            assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

            const bool consume = buttons[b][e];
            if(consume) buttons[b][e] = false;
            return consume;
        };

        /*!
         * \brief Set a button event.
         * 
         * \param b Button to set.
         * \param e Event to set.
         */
        inline static void set_button_event(const std::size_t& b, const std::size_t& e) {
            assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
            assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

            buttons[b][e] = true;
        };

        /*!
         * \brief Get the last key that was pressed.
         * 
         * \return Key code.
         */
        inline static const std::size_t get_last_keypress(void) {
            return last_keypress;
        };

        /*!
         * \brief Set the last key pressed.
         * 
         * \param lk Key code of last key pressed.
         */
        inline static void set_last_keypress(const std::size_t& lk) {
            last_keypress = lk;
        };

        /*!
         * \brief Get the last button that was pressed.
         * 
         * \return Button code.
         */
        inline static const std::size_t get_last_buttonpress(void) {
            return last_buttonpress;
        };

        /*!
         * \brief Set the last button pressed.
         * 
         * \param lk Code of last button pressed.
         */
        inline static void set_last_buttonpress(const std::size_t& lk) {
            last_buttonpress = lk;
        };

    private:
        inline input_flags() { unset_all(); };
        inline ~input_flags() { unset_all(); };

        inline static std::size_t last_keypress = 0;
        inline static std::size_t last_buttonpress = 0;

        inline static bool dflags[WTE_MAX_JOYSTICK_FLAGS][WTE_MAX_DIRECTON_FLAGS];

        inline static float angle[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static float pol_x[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static float pol_y[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };

        inline static bool buttons[WTE_MAX_INPUT_BUTTON_FLAGS][WTE_MAX_BUTTON_EVENT_FLAGS];
};

} //  end namespace wte

#endif
