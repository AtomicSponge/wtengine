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

#include <atomic>
#include <cassert>

namespace wte
{

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
 * \enum Input direction flags.
 */
enum WTE_DIRECTION_FLAG_ENUM {
    WTE_INPUT_DIRECTON_SET,     WTE_INPUT_DIRECTION_UNSET,
    WTE_MAX_DIRECTON_FLAGS
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
         * Set all input flags to false.
         * \param void
         * \return void
         */
        inline static void unset_all(void) {
            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                for(std::size_t d = 0; d < WTE_MAX_DIRECTON_FLAGS; d++)
                    dflags[j][d].store(false, std::memory_order_release);

            for(std::size_t j = 0; j < WTE_MAX_JOYSTICK_FLAGS; j++)
                angle[j].store(0.0f, std::memory_order_release);

            for(std::size_t b = 0; b < WTE_MAX_INPUT_BUTTON_FLAGS; b++)
                for(std::size_t e = 0; e < WTE_MAX_BUTTON_EVENT_FLAGS; e++)
                    buttons[b][e].store(false, std::memory_order_release);
        };

        /*!
         * Toggle flag to set.
         * \param j Joystick to set flag for.
         * \param d Direction flag to set.
         * \return void
         */
        inline static void joystick_toggle(const std::size_t& j, const std::size_t& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            assert(d < WTE_MAX_DIRECTON_FLAGS);
            dflags[j][d].store(true, std::memory_order_release);
        };

        /*!
         * \brief Check if a flag is set.
         * Unsets the flag if it is.
         * \param j Joystick to check.
         * \param d Direction flag to check.
         * \return True if set, false if not.
         */
        inline static const bool joystick_check(const std::size_t& j, const std::size_t& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            assert(d < WTE_MAX_DIRECTON_FLAGS);
            return dflags[j][d].exchange(false, std::memory_order_consume);
        };

        /*!
         * Set radians for a joystick.
         * \param j Joystick to set for.
         * \param a Angle in radians.
         */
        inline static void set_joystick_radians(const std::size_t& j, const float& a) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            angle[j].store(a, std::memory_order_release);
        };

        /*!
         * Set the direction of a joystick.
         * \param j Joystick to check.
         * \return Angle in radians.
         */
        inline static const float get_joystick_radians(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            return angle[j].load(std::memory_order_consume);
        };

        /*!
         * Set X pole of a joystick.
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_x(const std::size_t& j, const float& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            pol_x[j].store(d, std::memory_order_release);
        };

        /*!
         * Set Y pole of a joystick.
         * \param j Joystick to set.
         * \param d Direction to set.
         */
        inline static void set_joystick_pol_y(const std::size_t& j, const float& d) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            pol_y[j].store(d, std::memory_order_release);
        };

        /*!
         * Get X pole for a joystick.
         * \param j Joystick to set.
         * \return Joystick X direction.
         */
        inline static const float get_joystick_pol_x(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            return pol_x[j].load(std::memory_order_consume);
        };

        /*!
         * Get Y pole for a joystick.
         * \param j Joystick to set.
         * \return Joystick Y direction.
         */
        inline static const float get_joystick_pol_y(const std::size_t& j) {
            assert(j < WTE_MAX_JOYSTICK_FLAGS);
            return pol_y[j].load(std::memory_order_consume);
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

        inline static std::atomic<bool> dflags[WTE_MAX_JOYSTICK_FLAGS][WTE_MAX_DIRECTON_FLAGS];

        inline static std::atomic<float> angle[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static std::atomic<float> pol_x[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };
        inline static std::atomic<float> pol_y[WTE_MAX_JOYSTICK_FLAGS] = { 0.0f };

        inline static std::atomic<bool> buttons[WTE_MAX_INPUT_BUTTON_FLAGS][WTE_MAX_BUTTON_EVENT_FLAGS];
};

} //  end namespace wte

#endif
