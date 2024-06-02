/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_BACKGROUND_HPP)
#define WTE_CMP_BACKGROUND_HPP

#include <string>
#include <map>
#include <functional>

#include <allegro5/allegro.h>

#include "wtengine/cmp/gfx.hpp"

namespace wte::cmp::gfx {

/*!
 * \class background
 * \brief Component for storing a Background image and defining its animation process.
 */
class background final : public gfx {
  public:
    /*!
     * \brief Create a static Background component.
     * \param bmp Bitmap for static background.
     * \param l Background layer.
     * \param x X position of background.
     * \param y Y position of background.
     */
    background(
      wte_asset<ALLEGRO_BITMAP> bmp,
      const std::size_t& l,
      const float& x,
      const float& y
    ) : gfx(bmp, l, [](const entity_id& e_id) {}), pos_x(x), pos_y(y) {};

    /*!
     * \brief Create a Background component with custom animation.
     * \param bmp Bitmap for background.
     * \param l Background layer.
     * \param x X position of background.
     * \param y Y position of background.
     * \param func Animcation function.
     */
    background(
      wte_asset<ALLEGRO_BITMAP> bmp,
      const std::size_t& l,
      const float& x,
      const float& y,
      const std::function<void(const entity_id&)>& func
    ) : gfx(bmp, l, func), pos_x(x), pos_y(y) {};

    background() = delete;    //  Delete default constructor.
    ~background() = default;  //  Default destructor.

    float pos_x;  //!<  X position.
    float pos_y;  //!<  Y position.
};

}  //  end namespace wte::cmp

#endif
