/*
  WTEngine
  By:  Matthew Evans
  File:  key_flags.hpp

  See LICENSE.txt for copyright information

  Key flags
*/

#ifndef WTE_KEY_FLAGS_HPP
#define WTE_KEY_FLAGS_HPP

namespace wte
{

enum PLAYERKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_FIRE_1, KEY_FIRE_2, KEY_FIRE_3, MAX_KEY_FLAGS
};

//!
/*!
*/
class key_flags {
    public:
        inline static const bool is_set(const int flag) { return key[flag]; };
        inline static void set(const int flag, const bool set) { key[flag] = set; };

    private:
        static bool key[MAX_KEY_FLAGS];
};

inline bool key_flags::key[MAX_KEY_FLAGS] = { false, false, false, false, false, false, false };

} //  end namespace wte

#endif
