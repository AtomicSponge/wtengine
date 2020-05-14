/*!
 * \brief WTEngine | File:  spawn_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Spawn manager.
 */

#ifndef WTE_MGR_SPAWN_MANAGER_HPP
#define WTE_MGR_SPAWN_MANAGER_HPP

#include <string>
#include <map>
#include <functional>

#include "manager.hpp"
#include "entity_manager.hpp"
#include "../message.hpp"

namespace wte
{

namespace mgr
{

//! spawn_manager class
/*!
 * Spawn stuff
 */
class spawn_manager final : public manager<spawn_manager> {
    public:
        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline spawn_manager() { spawner.clear(); };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline ~spawn_manager() { spawner.clear(); };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline void process(const message_container messages, entity_manager& world) {
            for(auto it = messages.begin(); it != messages.end(); it++) {
                //
            }
        };

        /*!
         * spawn_manager
         * \param void
         * \return void
         */
        inline void add_spawn(std::string name, void func()) {
            spawner.insert(std::make_pair(name, func));
        };

    private:
        std::map<std::string, std::function<void()>> spawner;
};

template <> inline bool spawn_manager::manager<spawn_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
