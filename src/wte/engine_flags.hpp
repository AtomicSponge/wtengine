/*
  WTEngine
  By:  Matthew Evans
  File:  engine_flags.hpp

  See LICENSE.txt for copyright information

  System flags
*/

#ifndef WTE_ENGINE_FLAGS_HPP
#define WTE_ENGINE_FLAGS_HPP

namespace wte
{

enum ENGINE_FLAG_ENUM {
    IS_RUNNING,
    GAME_STARTED, GAME_MENU_OPENED,
    DRAW_HITBOX, DRAW_FPS,
    MAX_SYSTEM_FLAGS
};

//!
/*!
*/
class engine_flags {
    public:
        inline engine_flags() {};
        inline ~engine_flags() {};

        engine_flags(const engine_flags&) = delete;
        void operator=(engine_flags const&) = delete;

        inline static void set(const int f) { flags[f] = true; };
        inline static void unset(const int f) { flags[f] = false; };
        inline static const bool is_set(const int f) { return flags[f]; };

    private:
        static bool flags[MAX_SYSTEM_FLAGS];
};

inline bool engine_flags::flags[MAX_SYSTEM_FLAGS] = { false, false, false, false, false };

} //  end namespace wte

#endif
