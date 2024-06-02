/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/cmp/sprite.hpp"

namespace wte::cmp::gfx {

/*
 *
 */
sprite::sprite(
  wte_asset<ALLEGRO_BITMAP> bmp,
  const std::size_t& l,
  const float& sw, const float& sh,
  const float& dox, const float& doy,
  const std::size_t& rt) :
  gfx(bmp, l,[this](const entity_id& e_id) {
    //  Define sprite animation process.
    if(engine_time::check() % rate == 0) {
      //  Increment frame.
      current_frame++;
      //  Loop frame.
      if(current_frame > stop_frame) {
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
  if(rate == 0) rate = 1;
  sheet_width = al_get_bitmap_width(_bitmap.get());
  sheet_height = al_get_bitmap_height(_bitmap.get());
}

}  //  end namespace wte::cmp
