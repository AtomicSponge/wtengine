/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_CMP_OVERLAY_HPP)
#define WTE_CMP_OVERLAY_HPP

#include <string>
#include <map>
#include <functional>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "wtengine/cmp/gfx.hpp"

namespace wte::cmp::gfx {

/*!
 * \class overlay
 * \brief Component for storing an overlay image and defining its animation process.
 */
class overlay final : public gfx {

  public:
    /*!
     * \brief Create a new Overlay component.
     * \param bmp Bitmap asset to use.
     * \param font Font asset to use.
     * \param l Layer for sorting.
     * \param x Horizontal location of the overlay in pixels.
     * \param y Vertical location of the overlay in pixels.
     * \param func Function to define what is displayed in the overlay.
     */
    overlay(
      wte_asset<ALLEGRO_BITMAP> bmp,
      wte_asset<ALLEGRO_FONT> font,
      const std::size_t& l,
      const float& x,
      const float& y,
      const std::function<void(const entity_id&)>& func
    );

    overlay() = delete;    //  Delete default constructor.
    ~overlay() = default;  //  Default destructor.

    /*!
     * \brief Draw text on the overlay.
     * \param txt Text to be displayed.
     * \param color Allegro color object.
     * \param x Horizontal location of the text.
     * \param y Vertical location of the text.
     * \param f Text flags for drawing - see Allegro docs on al_draw_text.
     */
    void draw_text(
      const std::string& txt,
      const ALLEGRO_COLOR& color,
      const float& x,
      const float& y,
      const int& f
    );

    float pos_x;  //!<  X position.
    float pos_y;  //!<  Y position.

  private:
    wte_asset<ALLEGRO_FONT> overlay_font;  //  Font for overlay.
};

}  //  end namespace wte::cmp

#endif
