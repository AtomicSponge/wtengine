/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SCENE_HPP)
#define WTE_SCENE_HPP

#include <string>

namespace wte {

/*!
 * \class scene
 * \brief Extend this to create a scene.
 */
class scene {
  private:
    //

  protected:
    scene() = default;

  public:
    virtual ~scene() = default;             //  Default virtual destructor.
    scene(const scene&) = delete;           //  Delete copy constructor.
    void operator=(scene const&) = delete;  //  Delete assignment operator.

    virtual void load(void) {};
    virtual void unload(void) {};
    virtual void loop(void) {};

    const std::string name;  //!<  Scene name.
};

}  //  end namespace wte

#endif
