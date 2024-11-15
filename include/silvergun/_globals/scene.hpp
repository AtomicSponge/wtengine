/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_SCENE_HPP)
#define SLV_SCENE_HPP

#include <string>

#include "silvergun/_globals/handlers.hpp"

namespace slv {

/*!
 * \class scene
 * \brief Extend this to create a scene.
 */
class scene {
  protected:
    /*!
     * \brief Construct a new scene.
     * \param n Scene name.
     * \param s Scene scope.
     */
    scene(const std::string& n, const std::size_t& s) : name(n), scope(s) {
      assert(s == SCOPE_A || s == SCOPE_B || s == SCOPE_C &&
        "Scope must be one of the following: SCOPE_A, SCOPE_B, SCOPE_C");
    };

  public:
    virtual ~scene() = default;             //  Default virtual destructor.
    scene(const scene&) = delete;           //  Delete copy constructor.
    void operator=(scene const&) = delete;  //  Delete assignment operator.

    /*!
     * \brief Load the scene.
     */
    virtual void load(void) {};

    /*!
     * \brief Unload the scene.
     */
    virtual void unload(void) {};

    /*!
     * \brief Custom engine loop code.
     */
    virtual void loop(void) {};

    const std::string name;   //!<  Scene name.
    const std::size_t scope;  //!<  Scene handler scope.
};

}

#endif
