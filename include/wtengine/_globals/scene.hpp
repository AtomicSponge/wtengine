/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SCENE_HPP)
#define WTE_SCENE_HPP

#include <string>

#include "wtengine/_globals/handlers.hpp"

namespace wte {

/*!
 * \class scene
 * \brief Extend this to create a scene.
 */
class scene {
  protected:
    /*!
     * \brief
     * \param n Scene name.
     */
    scene(const std::string& name, const std::size_t& scope) : name(name), scope(scope) {
      assert(scope == SCOPE_A || scope == SCOPE_B || scope == SCOPE_C &&
        "Scope must be one of the following: SCOPE_A, SCOPE_B, SCOPE_C");
    };

  public:
    virtual ~scene() = default;             //  Default virtual destructor.
    scene(const scene&) = delete;           //  Delete copy constructor.
    void operator=(scene const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief
     */
    virtual void load(void) {};

    /*!
     * \brief
     */
    virtual void unload(void) {};

    /*!
     * \brief
     */
    virtual void handlers(void) {};

    /*!
     * \brief
     */
    virtual void loop(void) {};

    const std::string name;   //!<  Scene name.
    const std::size_t scope;  //!<  Scene handler scope.
};

}  //  end namespace wte

#endif
