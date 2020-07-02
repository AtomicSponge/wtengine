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

#include <bitset>
#include <atomic>
#include <cassert>

namespace wte
{

enum WTE_INPUT_FLAG_ENUM {
    WTE_INPUT_DIRECTIONAL_DOWN,     WTE_INPUT_DIRECTIONAL_UP,

    WTE_INPUT_UP_ON_DOWN,           WTE_INPUT_UP_ON_UP,
    WTE_INPUT_DOWN_ON_DOWN,         WTE_INPUT_DOWN_ON_UP,
    WTE_INPUT_LEFT_ON_DOWN,         WTE_INPUT_LEFT_ON_UP,
    WTE_INPUT_RIGHT_ON_DOWN,        WTE_INPUT_RIGHT_ON_UP,

    WTE_INPUT_ACTION_1_ON_DOWN,     WTE_INPUT_ACTION_1_ON_UP,
    WTE_INPUT_ACTION_2_ON_DOWN,     WTE_INPUT_ACTION_2_ON_UP,
    WTE_INPUT_ACTION_3_ON_DOWN,     WTE_INPUT_ACTION_3_ON_UP,
    WTE_INPUT_ACTION_4_ON_DOWN,     WTE_INPUT_ACTION_4_ON_UP,

    WTE_INPUT_MENU_ALT,
    WTE_INPUT_MENU_SELECT,
    WTE_INPUT_MENU_CLOSE,

    WTE_MAX_INPUT_FLAGS
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
        };

        inline static void set_radians(const float& a) {
            angle = a;
        };

        inline static const float get_radians(void) {
            return angle;
        };

        inline static void axis_set(void) {
            set.store(true, std::memory_order_relaxed);
        };

        inline static void axis_unset(void) {
            set.store(false, std::memory_order_relaxed);
        };

        inline static const bool axis_isset(void) {
            return set.load(std::memory_order_relaxed);
        };

    private:
        inline input_flags() { unset_all(); };
        inline ~input_flags() { unset_all(); };

        inline static std::atomic<float> angle = 0.0f;
        inline static std::atomic<bool> set = false;

        inline static std::atomic<bool> flags[WTE_MAX_INPUT_FLAGS] = {};
};

} //  end namespace wte

#endif
