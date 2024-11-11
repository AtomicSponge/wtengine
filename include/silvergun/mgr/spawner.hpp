/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_SPAWNER_HPP)
#define WTE_MGR_SPAWNER_HPP

#include <string>
#include <utility>
#include <map>
#include <functional>

#include "silvergun/mgr/manager.hpp"

#include "silvergun/_globals/message.hpp"
#include "silvergun/mgr/world.hpp"

namespace wte {
  class engine;
}

namespace wte::mgr {

/*!
 * \class spawner
 * \brief Create or delete entities while the engine is running.
 */
class spawner final : private manager<spawner> {
  friend class wte::engine;

  private:
    spawner() = default;
    ~spawner() = default;

    //  Takes spawner messages and processes.
    static void process_messages(const message_container& messages) {
      for (auto& m_it: messages) {
        if (m_it.get_cmd() == "new") {
          auto s_it = spawns.find(m_it.get_arg(0));
          if (s_it != spawns.end())
            //  Make sure the number of arguments match what's expected.
            //  Note that we do not count the first argument.
            if (m_it.num_args() == s_it->second.first + 1) {
              try {
                s_it->second.second(mgr::world::new_entity(), m_it.get_args());
              } catch(const std::exception& e) { throw e; }
            }
        }

        if (m_it.get_cmd() == "delete") {
          entity_id delete_entity_id = mgr::world::get_id(m_it.get_arg(0));
          if (delete_entity_id != wte::mgr::ENTITY_ERROR) {
            mgr::world::delete_entity(delete_entity_id);
          }
        }
      }
    };

    inline static std::map<
      const std::string,
      std::pair<
        const std::size_t,
        const std::function<void(const entity_id&, const msg_args&)>>> spawns;

  public:
    /*!
     * \brief Add a spawn to the spawner map.
     * 
     * Stores a function in the map for entity creation on demand.
     * 
     * \param name Reference name for the spwaner item.
     * \param num_args Number of arguments the spawn accepts.
     * \param func Function for creating the entity.
     * \return True if inserted into the spawn map, false if not.
     */
    static bool add(
      const std::string& name,
      const std::size_t& num_args,
      const std::function<void(const entity_id&, const msg_args&)>& func
    ) {
      auto ret = spawns.insert(std::make_pair(name, std::make_pair(num_args, func)));
      return ret.second;
    };

    /*!
     * \brief Delete a spawn.
     * \param name Name of spawn to delete.
     * \return True if removed, else false.
     */
    static bool remove(const std::string& name) {
      auto it = std::find_if (spawns.begin(), spawns.end(),
                            [&name](const auto& e){ return e.first == name; });
      if (it != spawns.end()) {
        spawns.erase(it);
        return true;
      }
      return false;
    };

    /*!
     * \brief Spawn entity.
     * \param name Name of entity to spawn.
     * \param args Arguments to entity creation.
     * \return True if spawned, else false.
     */
    static bool spawn(const std::string& name, const msg_args& args) {
      auto it = spawns.find(name);
      if (it != spawns.end()) {
        if (args.size() == it->second.first) {
          try {
            it->second.second(mgr::world::new_entity(), args);
          } catch(const std::exception& e) { throw e; }
          return true;
        }
      }
      return false;
    };
};

template <> bool manager<spawner>::initialized = false;

}  //  end namespace wte::mgr

#endif
