/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SYS_ANIMATE_HPP)
#define WTE_SYS_ANIMATE_HPP

#include "wtengine/sys/system.hpp"

namespace wte::sys::gfx {

/*!
 * \class animate
 * \brief Find the animate components and process them.
 */
class animate final : public system {
  public:
    animate() : system("animate") {};
    ~animate() = default;

    /*!
     * \brief Gets all animation components and processes their run members.
     * 
     * The entity must also have the visible component and is set visible to be drawn.
     */
    void run(void) override {
      component_container<cmp::gfx::gfx> animation_components =
        mgr::world::set_components<cmp::gfx::gfx>();

      for (auto& it: animation_components)
        if (it.second->visible) it.second->animate(it.first);
    };
};

}  //  end namespace wte::sys

#endif
