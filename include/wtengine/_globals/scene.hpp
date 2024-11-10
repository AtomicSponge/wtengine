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
  protected:
    /*!
     * \brief
     * \param n Scene name.
     */
    scene(const std::string& name) : name(name) {};

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
    virtual void loop(void) {};

    /*!
     * \brief
     */
    //virtual void draw(void) {};

    const std::string name;  //!<  Scene name.
};

}  //  end namespace wte

#endif
