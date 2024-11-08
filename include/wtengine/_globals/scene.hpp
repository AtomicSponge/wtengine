/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_SCENE_HPP)
#define WTE_SCENE_HPP

namespace wte {

/*!
 * \class scene
 * \brief Extend this to create a scene.
 */
class scene {
  private:
    virtual void load(void) {};
    virtual void unload(void) {};
    virtual void loop(void) {};

  protected:
    scene() = default;

  public:
    virtual ~scene() = default;
};

}  //  end namespace wte

#endif
