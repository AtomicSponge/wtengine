/*!
 * WTEngine | File:  engine_flags.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#ifndef WTE_ENGINE_FLAGS_HPP
#define WTE_ENGINE_FLAGS_HPP

#include <cassert>

namespace wte
{

/*!
 * \enum ENGINE_FLAG_ENUM
 * List of engine flags.
 */
enum ENGINE_FLAG_ENUM {
    IS_RUNNING,
    GAME_STARTED, GAME_MENU_OPENED,
    DRAW_FPS,
    MAX_SYSTEM_FLAGS
};

/*!
 * \class engine_flags
 * \brief Collection of flags to store engine state.
 */
class engine_flags final {
    public:
        //!  Remove copy constructor
        engine_flags(const engine_flags&) = delete;
        //!  Remove assignment operator
        void operator=(engine_flags const&) = delete;

        /*!
         * \brief Set a flag to true.
         * 
         * \param i Index of the flag.
         */
        static void set(const std::size_t& i);

        /*!
         * \brief Set a flag to false.
         * 
         * \param i Index of the flag.
         */
        static void unset(const std::size_t& i);

        /*!
         * \brief Check the state of a flag.
         * 
         * \param i Index of the flag.
         * \return True if the flag is set, false if not.
         */
        static const bool is_set(const std::size_t& i);

    private:
        inline engine_flags() { unset_all(); };
        inline ~engine_flags() { unset_all(); };

        /*!
         * \brief Set all engine flags to false.
         */
        static void unset_all(void);

        static bool flags[MAX_SYSTEM_FLAGS];
};

} //  end namespace wte

#endif
