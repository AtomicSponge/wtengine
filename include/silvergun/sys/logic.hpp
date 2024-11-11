/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_SYS_LOGIC_HPP)
#define SLV_SYS_LOGIC_HPP

#include "silvergun/sys/system.hpp"

namespace slv::sys {

/*!
 * \class logic
 * \brief Processes entities that have ai components.
 * 
 * Also sends messages to entities with dispatch components.
 */
class logic final : public system {
  public:
    logic() : system("logic") {};
    ~logic() = default;

    /*!
     * \brief Finds all entities with an ai component and processes their logic.
     */
    void run(void) override {
      //  Find the entities with the input handler component
      component_container<cmp::ai> ai_components =
        mgr::world::set_components<cmp::ai>();

      //  Process enabled or disabled ai
      for (auto& it: ai_components) {
        (it.second->enabled ?
          it.second->enabled_ai(it.first) :
          it.second->disabled_ai(it.first));
      }
    };
};

}

#endif
