/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(SLV_CMP_SPRITE_HPP)
#define SLV_CMP_SPRITE_HPP

#include <string>
#include <utility>
#include <map>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "silvergun/cmp/gfx.hpp"

#include "silvergun/_debug/exceptions.hpp"
#include "silvergun/_globals/_defines.hpp"
#include "silvergun/_globals/engine_time.hpp"
#include "silvergun/mgr/world.hpp"

namespace slv::mgr::gfx {
  class renderer;
}

namespace slv::cmp::gfx {

/*!
 * \class sprite
 * \brief Component for loading a sprite sheet and storing its animation frames.
 */
class sprite final : public gfx {
  friend class mgr::gfx::renderer;

  private:
    //  Animation cycle index.
    std::map<
      const std::string,
      const std::pair<const std::size_t, const std::size_t>
    > cycles;

    float sprite_width, sprite_height;      //  Sprite width & height.
    float draw_offset_x, draw_offset_y;     //  Sprite draw offset x/y.
    float sprite_x, sprite_y;               //  Sprite location.
    int sheet_width, sheet_height;          //  Sprite sheet size (w/h).
    std::size_t start_frame, stop_frame;    //  Current start/stop frame postitions.
    std::size_t current_frame, rate;        //  Current frame counter and frame rate.

  public:
    /*!
     * \brief Create a new Sprite component.
     * \param l Layer for sorting.
     * \param sw Sprite width.
     * \param sh Sprite height.
     * \param dox Horizontal draw offset in relation to entity location.
     * \param doy Verticle draw offset in relation to entity location.
     * \param rt Rate in engine timer ticks for animation.
     */
    sprite(
      slv_asset<ALLEGRO_BITMAP> bmp,
      const std::size_t& l,
      const float& sw,
      const float& sh,
      const float& dox,
      const float& doy,
      const std::size_t& rt
    ) :gfx(bmp, l,[this](const entity_id& e_id) {
        //  Define sprite animation process.
        if (engine_time::check() % rate == 0) {
          //  Increment frame.
          current_frame++;
          //  Loop frame.
          if (current_frame > stop_frame) {
              current_frame = start_frame;
          }
          //  Calculate the X position in the sprite sheet.
          sprite_x = (float)((int)(current_frame * sprite_width + sheet_width) % sheet_width);
          //  Calculate the Y position in the sprite sheet.
          sprite_y = (float)((int)((current_frame * sprite_width) / sheet_width) * sprite_height);
        }
      }),
      sprite_width(sw), sprite_height(sh), draw_offset_x(dox), draw_offset_y(doy),
      sprite_x(0.0f), sprite_y(0.0f),
      start_frame(0), stop_frame(0), current_frame(0), rate(rt)
    {
      if (rate == 0) rate = 1;
      sheet_width = al_get_bitmap_width(_bitmap.get());
      sheet_height = al_get_bitmap_height(_bitmap.get());
    };

    sprite() = delete;    //  Delete default constructor.
    ~sprite() = default;  //  Default destructor.

    /*!
     * \brief Add animation cycle.
     * \param name Name of cycle.
     * \param start Start cell of cycle.
     * \param stop End cell of cycle.
     * \return True if created, false if not.
     */
    bool add_cycle(
      const std::string& name,
      const std::size_t& start,
      const std::size_t& stop
    ) {
      auto ret = cycles.insert(std::make_pair(name, std::make_pair(start, stop)));
      return ret.second;
    };

    /*!
     * \brief Set the active cycle.
     * \param name Name of cycle to set.
     * \return True if set, false if not.
     */
    bool set_cycle(const std::string& name) {
      auto it = cycles.find(name);
      if (it != cycles.end()) {
        start_frame = it->second.first;
        stop_frame = it->second.second;
        return true;
      } else return false;
    };
};

}

#endif
