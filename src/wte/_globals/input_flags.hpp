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

namespace wte
{

enum WTE_INPUT_FLAG_ENUM {
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

//!  Input flags
/*!
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
         * \param f Flag to set.
         * \return void
         */
        inline static void toggle(const std::size_t& f) {
            flags[f].store(true, std::memory_order_release);
        };

        /*!
         * \brief Check if a flag is set.
         * Unsets the flag if it is.
         * \param f Flag to check.
         * \return True if set, false if not.
         */
        inline static const bool check(const std::size_t& f) {
            const bool consume = flags[f].load(std::memory_order_consume);
            if(consume) flags[f].store(false, std::memory_order_release);
            return consume;
        };

        /*!
         * Set all input flags to false.
         * \param void
         * \return void
         */
        inline static void unset_all(void) {
            for(std::size_t i = 0; i < WTE_MAX_INPUT_FLAGS; i++)
                flags[i].store(false, std::memory_order_release);
        }

    private:
        inline input_flags() { unset_all(); };
        inline ~input_flags() { unset_all(); };

        inline static std::atomic<bool> flags[WTE_MAX_INPUT_FLAGS] = {};
};

} //  end namespace wte

#endif
