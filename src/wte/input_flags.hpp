/*
  WTEngine
  By:  Matthew Evans
  File:  input_flags.hpp

  See LICENSE.txt for copyright information

  Input flags
*/

#ifndef WTE_INPUT_FLAGS_HPP
#define WTE_INPUT_FLAGS_HPP

#include <atomic>

namespace wte
{

enum INPUT_FLAG_ENUM {
   INPUT_UP, INPUT_DOWN, INPUT_LEFT, INPUT_RIGHT,
   INPUT_ACTION_1, INPUT_ACTION_2, INPUT_ACTION_3,
   INPUT_MENU_SELECT,
   MAX_INPUT_FLAGS
};

//!  Input flags
/*!
  Colletion of flags to store input state
*/
class input_flags final {
    public:
        //!  Remove copy constructor
        input_flags(const input_flags&) = delete;
        //!  Remove assignment operator
        void operator=(input_flags const&) = delete;

        //!  Set a flag to true
        inline static void set(const std::size_t f) { flags[f] = true; };
        //!  Set a flag to false
        inline static void unset(const std::size_t f) { flags[f] = false; };
        //!  Check the state of a flag
        inline static const bool is_set(const std::size_t f) { return flags[f]; };

        //!  Set all input flags to false
        inline static void unset_all(void) {
            for(std::size_t i = 0; i < MAX_INPUT_FLAGS; i++) flags[i] = false;
        }

    private:
        inline input_flags() {};
        inline ~input_flags() {};

        static std::atomic<bool> flags[MAX_INPUT_FLAGS];
};

inline std::atomic<bool> input_flags::flags[MAX_INPUT_FLAGS] = {};

} //  end namespace wte

#endif
