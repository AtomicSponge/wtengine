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
    //

  protected:
    scene() = default;

  public:
    virtual ~scene() = default;

    virtual void load(void) {};
    virtual void unload(void) {};
    virtual void loop(void) {};
};

}  //  end namespace wte

#endif
