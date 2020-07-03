/*!
 * \brief WTEngine | File:  input_flags.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Input flags.
 */

#ifndef WTE_INPUT_FLAGS_HPP
#define WTE_INPUT_FLAGS_HPP

#include <cmath>

#include <atomic>
#include <cassert>

namespace wte
{

/*!
 * \enum Input directions.
 */
enum WTE_INPUT_FLAG_ENUM {
    WTE_INPUT_DIRECTON_SET,     WTE_INPUT_DIRECTION_UNSET,
    WTE_MAX_INPUT_FLAGS
};

/*!
 * \enum List of buttons.
 */
enum WTE_INPUT_BUTTON_FLAG_ENUM {
    WTE_INPUT_BUTTON_UP,        WTE_INPUT_BUTTON_DOWN,
    WTE_INPUT_BUTTON_LEFT,      WTE_INPUT_BUTTON_RIGHT,

    WTE_INPUT_BUTTON_1,         WTE_INPUT_BUTTON_3,
    WTE_INPUT_BUTTON_2,         WTE_INPUT_BUTTON_4,

    WTE_INPUT_MENU_ALT,
    WTE_INPUT_MENU_SELECT,
    WTE_INPUT_MENU_CLOSE,

    WTE_MAX_INPUT_BUTTON_FLAGS
};

/*!
 * \enum Button events.
 */
enum WTE_BUTTON_EVENT_FLAGS {
    WTE_BUTTON_EVENT_DOWN,
    WTE_BUTTON_EVENT_UP,
    WTE_MAX_BUTTON_EVENT_FLAGS
};

/*!
 * \enum Joysticks.
 */
enum WTE_JOYSTICK_FLAGS {
    WTE_JOYSTICK_A,
    //WTE_JOYSTICK_B,
    //WTE_JOYSTICK_C,
    WTE_MAX_JOYSTICK_FLAGS
};

/*!
 * \class Input flags
 * Colletion of flags to store input state.
 */
class input_flags final {
    public:
        //!  Remove copy constructor
        input_flags(const input_flags&) = delete;
        //!  Remove assignment operator
        void operator=(input_flags const&) = delete;

        /*!
         * Toggle flag to set.
         * \param i Index of flag to set.
         * \return void
         */
        inline static void toggle(const std::size_t& i) {
            assert(i < WTE_MAX_INPUT_FLAGS);
            flags[i].store(true, std::memory_order_release);
        };

        /*!
         * \brief Check if a flag is set.
         * Unsets the flag if it is.
         * \param i Index of flag to check.
         * \return True if set, false if not.
         */
        inline static const bool check(const std::size_t& i) {
            assert(i < WTE_MAX_INPUT_FLAGS);
            return flags[i].exchange(false, std::memory_order_consume);
        };

        /*!
         * Set all input flags to false.
         * \param void
         * \return void
         */
        inline static void unset_all(void) {
            for(std::size_t i = 0; i < WTE_MAX_INPUT_FLAGS; i++)
                flags[i].store(false, std::memory_order_release);

            for(std::size_t i = 0; i < WTE_MAX_JOYSTICK_FLAGS; i++)
                angle[i].store(0.0f, std::memory_order_release);

            for(std::size_t i = 0; i < WTE_MAX_INPUT_BUTTON_FLAGS; i++)
                for(std::size_t e = 0; e < WTE_MAX_BUTTON_EVENT_FLAGS; e++)
                    buttons[i][e].store(false, std::memory_order_release);
        };

        /*!
         * Set radians for a joystick.
         * \param i Joystick to set for.
         * \param a Angle in radians.
         */
        inline static void set_joystick_radians(const std::size_t& i, const float& a) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            angle[i].store(a, std::memory_order_release);
        };

        /*!
         * Set the direction of a joystick.
         * \param i Joystick to check.
         * \return Angle in radians.
         */
        inline static const float get_joystick_radians(const std::size_t& i) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            return angle[i].load(std::memory_order_consume);
        };

        /*!
         * Set X pole of a joystick.
         * \param i Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_x(const std::size_t& i, const float& d) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            pol_x[i].store(d, std::memory_order_release);
        };

        /*!
         * Set Y pole of a joystick.
         * \param i Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_y(const std::size_t& i, const float& d) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            pol_y[i].store(d, std::memory_order_release);
        };

        /*!
         * Get X pole for a joystick.
         * \param i Joystick to set.
         * \return Joystick X direction.
         */
        inline static const float get_joystick_pol_x(const std::size_t& i) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            return pol_x[i].load(std::memory_order_consume);
        };

        /*!
         * Get Y pole for a joystick.
         * \param i Joystick to set.
         * \return Joystick Y direction.
         */
        inline static const float get_joystick_pol_y(const std::size_t& i) {
            assert(i < WTE_MAX_JOYSTICK_FLAGS);
            return pol_y[i].load(std::memory_order_consume);
        };

        /*!
         * Check if a button event is set.
         * \param b Button to check.
         * \param e Event to check.
         */
        inline static const bool check_button_event(const std::size_t& b, const std::size_t& e) {
            assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
            assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

            return buttons[b][e].exchange(false, std::memory_order_consume);
        };

        /*!
         * Set a button event.
         * \param b Button to set.
         * \param e Event to set.
         */
        inline static void set_button_event(const std::size_t& b, const std::size_t& e) {
            assert(b < WTE_MAX_INPUT_BUTTON_FLAGS);
            assert(e < WTE_MAX_BUTTON_EVENT_FLAGS);

            buttons[b][e].store(true, std::memory_order_release);
        };

    private:
        inline input_flags() { unset_all(); };
        inline ~input_flags() { unset_all(); };

        inline static std::atomic<bool> flags[WTE_MAX_INPUT_FLAGS] = {};

        inline static std::atomic<float> angle[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static std::atomic<float> pol_x[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static std::atomic<float> pol_y[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };

        inline static std::atomic<bool> buttons[WTE_MAX_INPUT_BUTTON_FLAGS][WTE_MAX_BUTTON_EVENT_FLAGS];
};

} //  end namespace wte

#endif
