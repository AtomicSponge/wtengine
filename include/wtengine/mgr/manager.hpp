/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_MANAGER_HPP)
#define WTE_MGR_MANAGER_HPP

#include "wtengine/_debug/exceptions.hpp"

namespace wte::mgr {

/*!
 * \class manager
 * \brief Manager base class, restricted to one instance.
 * \tparam D Manager that is deriving the class.
 */
template <class D>
class manager {
  private:
    static bool initialized;  //  Force single instance.

  protected:
    /*!
     * \brief Create a new engine manager.
     * \exception runtime_error Manager is already running.
     */
    manager() {
      if (initialized == true) throw engine_error("An instance of this manager is already running!");
      initialized = true;
    };

  public:
    virtual ~manager() = default;             //  Default virtual destructor.
    manager(const manager&) = delete;         //  Delete copy constructor.
    void operator=(manager const&) = delete;  //  Delete assignment operator.
};

}  //  end namespace wte::mgr

#endif
