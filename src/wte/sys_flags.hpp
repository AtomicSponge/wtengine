/*
  WTEngine
  By:  Matthew Evans
  File:  sys_flags.hpp

  See LICENSE.txt for copyright information

  System flags
*/

#ifndef WTE_SYS_FLAGS_HPP
#define WTE_SYS_FLAGS_HPP

namespace wte
{

enum GLOBAL_SYSTEM_FLAGS {
    IS_RUNNING, GAME_STARTED, GAME_MENU_OPENED, DRAW_HITBOX, DRAW_FPS, MAX_SYSTEM_FLAGS
};

//!
/*!
*/
class sys_flags {
    public:
        inline sys_flags() {};
        inline ~sys_flags() {};

        sys_flags(const sys_flags&) = delete;
        void operator=(sys_flags const&) = delete;

        inline static void set(const int flag) { sys_flag[flag] = true; };
        inline static void unset(const int flag) { sys_flag[flag] = false; };
        inline static const bool is_set(const int flag) { return sys_flag[flag]; };

    private:
        static bool sys_flag[MAX_SYSTEM_FLAGS];
};

inline bool sys_flags::sys_flag[MAX_SYSTEM_FLAGS] = { false, false, false, false, false };

} //  end namespace wte

#endif
