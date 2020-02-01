/*
  WTEngine
  By:  Matthew Evans
  File:  input_flags.hpp

  See LICENSE.txt for copyright information

  Input flags
*/

#ifndef WTE_INPUT_FLAGS_HPP
#define WTE_INPUT_FLAGS_HPP

namespace wte
{

enum INPUT_FLAG_ENUM {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
   KEY_FIRE_1, KEY_FIRE_2, KEY_FIRE_3,
   INPUT_MENU_SELECT,
   MAX_KEY_FLAGS
};

//!
/*!
*/
class input_flags {
    public:
        inline input_flags() {};
        inline ~input_flags() {};

        input_flags(const input_flags&) = delete;
        void operator=(input_flags const&) = delete;

        inline static void set(const int f) { flags[f] = true; };
        inline static void unset(const int f) { flags[f] = false; };
        inline static const bool is_set(const int f) { return flags[f]; };

    private:
        static bool flags[MAX_KEY_FLAGS];
};

inline bool input_flags::flags[MAX_KEY_FLAGS] = { false, false, false, false, false, false, false };

} //  end namespace wte

#endif
