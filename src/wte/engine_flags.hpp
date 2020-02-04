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
    WAIT_FOR_VSYNC,
    MAX_SYSTEM_FLAGS
};

//!  Engine flags
/*!
  Collection of flags to store engine state
*/
class engine_flags final {
    public:
        inline engine_flags() {};
        inline ~engine_flags() {};

        //!  Remove copy constructor
        engine_flags(const engine_flags&) = delete;
        //!  Remove assignment operator
        void operator=(engine_flags const&) = delete;

        //!  Set a flag to true
        inline static void set(const int f) { flags[f] = true; };
        //!  Set a flag to false
        inline static void unset(const int f) { flags[f] = false; };
        //!  Check the state of a flag
        inline static const bool is_set(const int f) { return flags[f]; };

    private:
        static bool flags[MAX_SYSTEM_FLAGS];
};

inline bool engine_flags::flags[MAX_SYSTEM_FLAGS] = {};

} //  end namespace wte

#endif
