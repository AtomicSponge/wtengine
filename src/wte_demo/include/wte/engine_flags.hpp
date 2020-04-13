/*!
 * \brief WTEngine | File:  engine_flags.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Engine flags.
 */

#ifndef WTE_ENGINE_FLAGS_HPP
#define WTE_ENGINE_FLAGS_HPP

#include <atomic>

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
        //!  Remove copy constructor
        engine_flags(const engine_flags&) = delete;
        //!  Remove assignment operator
        void operator=(engine_flags const&) = delete;

        //!  Set a flag to true
        inline static void set(const std::size_t f) { flags[f] = true; };
        //!  Set a flag to false
        inline static void unset(const std::size_t f) { flags[f] = false; };
        //!  Check the state of a flag
        inline static const bool is_set(const std::size_t f) { return flags[f]; };

    private:
        inline engine_flags() {};
        inline ~engine_flags() {};

        static std::atomic<bool> flags[MAX_SYSTEM_FLAGS];
};

inline std::atomic<bool> engine_flags::flags[MAX_SYSTEM_FLAGS] = {};

} //  end namespace wte

#endif
