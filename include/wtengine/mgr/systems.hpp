/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_SYSTEMS_HPP)
#define WTE_MGR_SYSTEMS_HPP

#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/sys/system.hpp"

namespace wte {
  class engine;
}

namespace wte::mgr {

/*!
 * \class systems
 * \brief Store the configured systems and process their runs and dispatches.
 */
class systems final : private manager<systems> {
  friend class wte::engine;

  public:
    /*!
     * \brief Add a new system to the manager.
     * 
     * Enters system into the vector of systems.
     * Systems run in the order they were added.
     * Can fail if the system exists or if the game is running.
     *
     * \param new_system System to add.
     * \return True if added, false if not.
     */
    template <typename T, typename... Args>
    static bool add(Args... args) {
      if(finalized == true) return false;
      for(auto& it: _systems) {
        auto& r = *it.get();
        if(typeid(r) == typeid(T)) return false;
      }
      _systems.push_back(std::make_unique<T>(args...));
      return true;
    };

  private:
    systems() = default;
    ~systems() = default;

    //  Clear the system manager and allow systems to be loaded again.
    static void clear(void) {
      _systems.clear();
      finalized = false;
    };

    //  Check if systems were loaded into the manager.
    static bool empty(void) { return (_systems.empty()); };

    //  Run all systems.
    static void run(void) {
      for(auto& it: _systems)
        try { 
          (it)->run();
        } catch(const std::exception& e) { throw e; }
    };

    // Store the vector of systems.
    static std::vector<sys::system_uptr> _systems;
    //  Flag to disallow loading of additional systems.
    inline static bool finalized = false;
};

template <> bool manager<systems>::initialized = false;

}  //  end namespace wte::mgr

#endif
