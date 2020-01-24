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
        inline key_flags() {};
        inline ~key_flags() {};

        key_flags(const key_flags&) = delete;
        void operator=(key_flags const&) = delete;

        inline static void set(const int flag) { key[flag] = true; };
        inline static void unset(const int flag) { key[flag] = false; };
        inline static const bool is_set(const int flag) { return key[flag]; };

    private:
        static bool key[MAX_KEY_FLAGS];
};

inline bool key_flags::key[MAX_KEY_FLAGS] = { false, false, false, false, false, false, false };

} //  end namespace wte

#endif
