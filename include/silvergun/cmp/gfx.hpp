/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_ANIMATOR_HPP)
#define WTE_CMP_ANIMATOR_HPP

#include <functional>

#include <allegro5/allegro.h>

#include "silvergun/cmp/component.hpp"

#include "silvergun/_globals/wte_asset.hpp"
#include "silvergun/mgr/world.hpp"

namespace wte::mgr::gfx {
  class renderer;
}

namespace wte::sys::gfx {
  class animate;
}

namespace wte::cmp::gfx {

/*!
 * \class gfx
 * \brief Interface class for components that display graphics.
 */
class gfx : public component {
  friend class mgr::gfx::renderer;
  friend class sys::gfx::animate;

  private:
    bool tinted;               //  Flag to set tint.
    ALLEGRO_COLOR tint_color;  //  Color of tint.

    //  Animation function.
    const std::function<void(const entity_id&)> animate;

  protected:
    /*!
     * \brief Extend to create a gfx component.
     * \param bmp Bitmap asset to use.
     * \param l Layer position.
     * \param func Animation function.
     */
    gfx(
      wte_asset<ALLEGRO_BITMAP> bmp,
      const std::size_t& l,
      const std::function<void(const entity_id&)>& func
    ) : layer(l), visible(true), rotated(false), direction(0.0f),
        scale_factor_x(1.0f), scale_factor_y(1.0f),
        _bitmap(bmp), tinted(false), animate(func) {};

    //!  Stores the bitmap used by the animator.
    wte_asset<ALLEGRO_BITMAP> _bitmap;

  public:
    gfx() = delete;            //  Delete default constructor.
    virtual ~gfx() = default;  //  Default virtual destructor.

    /*!
     * \brief Overload < operator to sort by layer value.
     * \param a Object to compare to.
     * \return True if <, false if >
     */
    bool operator<(const gfx& a) const {
        return layer < a.layer;
    };

    /*!
     * \brief Set drawing to the internal bitmap.
     */
    void set_drawing(void) { al_set_target_bitmap(_bitmap.get()); };

    /*!
     * \brief Set a tint color.
     * \param c Allegro color.
     */
    void set_tint(const ALLEGRO_COLOR& c) {
      tint_color = c;
      tinted = true;
    };

    /*!
     * \brief Get the tint color.
     * \return Allegro color.
     */
    const ALLEGRO_COLOR get_tint(void) const { return tint_color; };

    /*!
     * \brief Clear tint color.
     */
    void clear_tint(void) { tinted = false; };

    std::size_t layer;     //!<  Layer position.
    bool visible;          //!<  Flag to set visibility.
    bool rotated;          //!<  Rotation flag.
    float direction;       //!<  Direction to draw when rotated.
    float scale_factor_x;  //!<  X scale factor.
    float scale_factor_y;  //!<  Y scale factor.
};

}  //  end namespace wte::cmp

#endif
