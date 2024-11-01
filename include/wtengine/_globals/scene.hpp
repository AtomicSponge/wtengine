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
  public:
    virtual ~scene() = default;

  protected:
    scene() = default;

  private:
    //
};

}  //  end namespace wte

#endif
