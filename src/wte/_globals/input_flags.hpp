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
   WTE_INPUT_UP, WTE_INPUT_DOWN, WTE_INPUT_LEFT, WTE_INPUT_RIGHT,

   WTE_INPUT_ACTION_1, WTE_INPUT_ACTION_2, WTE_INPUT_ACTION_3,
   WTE_INPUT_ACTION_4, WTE_INPUT_ACTION_5, WTE_INPUT_ACTION_6,
   WTE_INPUT_ACTION_7, WTE_INPUT_ACTION_8, WTE_INPUT_ACTION_9,

   WTE_INPUT_MENU_SELECT, WTE_INPUT_MENU_ALT, WTE_INPUT_MENU_CLOSE,

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
         * Set a flag to true.
         * \param f Index of the flag.
         * \return void
         */
        inline static void set(const std::size_t& f) {
            flags[f].store(true, std::memory_order_release);
        };

        /*!
         * Set a flag to false.
         * \param f Index of the flag.
         * \return void
         */
        inline static void unset(const std::size_t& f) {
            flags[f].store(false, std::memory_order_release);
        };

        /*!
         * Check the state of a flag.
         * \param f Index of the flag.
         * \return True if set, false if not set.
         */
        inline static const bool is_set(const std::size_t& f) {
            return flags[f].load(std::memory_order_consume);
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
