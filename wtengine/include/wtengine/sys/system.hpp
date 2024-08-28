/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SYS_SYSTEM_HPP)
#define WTE_SYS_SYSTEM_HPP

#include <string>
#include <memory>

#include "wtengine/cmp/_components.hpp"
#include "wtengine/mgr/messages.hpp"
#include "wtengine/mgr/world.hpp"

namespace wte::sys {

/*!
 * \class system
 * \brief Interface class for creating Systems.
 */
class system {
  public:
    virtual ~system() = default;             //  Default virtual destructor.
    system(const system&) = delete;          //  Delete copy constructor.
    void operator=(system const&) = delete;  //  Delete assignment operator.

    //!  Override this to create custom System run method.
    virtual void run(void) = 0;

    const std::string name;  //!<  System name.

  protected:
    /*!
     * \brief Create a new timed System object.
     * \param n System name.
     */
    system(
      const std::string& n
    ) : name(n) {};
};

/*!
 * \typedef std::unique_ptr<system> system_uptr
 * System unique pointer.
 */
using system_uptr = std::unique_ptr<system>;

}  //  end namespace wte::sys

#endif
