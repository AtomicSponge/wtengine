/*!
 * \brief WTEngine | File:  render_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Render manager.
 */

#ifndef WTE_MGR_RENDER_MANAGER_HPP
#define WTE_MGR_RENDER_MANAGER_HPP

#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "manager.hpp"
#include "engine_time.hpp"

#include "../wte_global_defines.hpp"
#include "../engine_cfg_map.hpp"
#include "../engine_flags.hpp"
#include "../cmp/components.hpp"
#include "../alert.hpp"
#include "menu_manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace mgr
{

//!  Container for an entity and component pair.  Used for sorting.
typedef std::pair<entity, cmp::component_sptr> entity_component_pair;
//!  Constant iterator for the entity/component pair.
typedef std::set<entity_component_pair>::const_iterator ec_pair_citerator;
//!  Function wrapper used to define entity sorting.
typedef std::function<const bool(entity_component_pair, entity_component_pair)> render_comparator;

//! render_manager class
/*!
 * An object that handles drawing the world to the screen.
 */
class render_manager final : public manager<render_manager>, private engine_time {
    public:
        /*!
         * render_manager constructor.
         * Generates the render_manager object.
         * \param void
         * \return void
         */
        inline render_manager() : fps_counter(0), fps(0), screen_w(0), screen_h(0), scale_factor(1.0) {
            title_bmp = NULL;
            arena_bmp = NULL;
            render_tmp_bmp = NULL;

            overlay_font = NULL;

            fps_timer = NULL;
            fps_event_queue = NULL;

            //  Define render comparator as lambda function that sorts components.
            comparator =
                [](const entity_component_pair r_element1, const entity_component_pair r_element2) {
                    return r_element1.second < r_element2.second;
                };
        };

        /*!
         * render_manager destructor.
         * Cleans up the render_manager object.
         * \param void
         * \return void
         */
        inline ~render_manager() {
            //al_destroy_bitmap(title_bmp);
            //al_destroy_bitmap(arena_bmp);
            //al_destroy_bitmap(render_tmp_bmp);

            al_destroy_font(overlay_font);

            al_destroy_event_queue(fps_event_queue);
            al_destroy_timer(fps_timer);
        };

        /*!
         * Initialize the render_manager.
         * Pass an Allegro font for the render_manager to use.
         * \param font Allegro font to use for the renderer.
         * \return void
         */
        inline void initialize(ALLEGRO_FONT* font) {
            if(arena_w == 0 || arena_h == 0) throw std::runtime_error("Arena size not defined!");
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            arena_bmp = al_create_bitmap(arena_w, arena_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            arena_created = true;

            title_bmp = al_create_bitmap(1, 1);
            al_set_target_bitmap(title_bmp);
            al_clear_to_color(WTE_COLOR_BLACK);

            overlay_font = font;
            fps_timer = al_create_timer(1);
            fps_event_queue = al_create_event_queue();
            al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));
            al_start_timer(fps_timer);
        };

        /*!
         * Set the title screen.
         * \param fname Filename of title screen.
         * \return True if file found, false if not found.
         */
        inline bool set_title_screen(const std::string fname) {
            al_destroy_bitmap(title_bmp);
            ALLEGRO_FILE* file;
            file = al_fopen(fname.c_str(), "rb");
            if(!file) {
                al_fclose(file);
                return false;
            }
            title_bmp = al_load_bitmap_f(file, fname.substr(fname.find("."), fname.length()).c_str());
            al_fclose(file);
            return true;
        };

        /*!
         * Inform the renderer of the screen resolution.
         * \param w Resolution width in pixels.
         * \param h Resolution height in pixels.
         * \return void
         */
        inline void update_resolution(const int w, const int h) {
            screen_w = w;
            screen_h = h;

            scale_factor = 1.0;
        }

        /*!
         * \brief Set the arena size.
         * Once the renderer has been initialized it can not be changed.
         * This should be called during engine initialization before the main object is created.
         * \param w Arena width in pixels.
         * \param h Arena height in pixels.
         * \return void
         */
        inline static void set_arena_size(const int w, const int h) {
            if(!arena_created) {
                arena_w = w;
                arena_h = h;
            }
        };

        /*!
         * Get the arnea width.
         * \param void
         * \return The arena width in pixels.
         */
        inline static const int get_arena_width(void) { return arena_w; };

        /*!
         * Get the arena height.
         * \param void
         * \return The arena height in pixels.
         */
        inline static const int get_arena_height(void) { return arena_h; };

        /*!
         * Render method - Draw the game screen.
         * \param void
         * \return void
         */
        void render(const menu_manager&, const entity_manager&);

    private:
        ALLEGRO_BITMAP* title_bmp;
        ALLEGRO_BITMAP* arena_bmp;
        ALLEGRO_BITMAP* render_tmp_bmp;
        ALLEGRO_FONT* overlay_font;

        ALLEGRO_TIMER* fps_timer;
        ALLEGRO_EVENT_QUEUE* fps_event_queue;
        ALLEGRO_EVENT fps_event;

        render_comparator comparator;

        std::size_t fps_counter, fps;

        int screen_w, screen_h;
        float scale_factor;

        inline static int arena_w = 0, arena_h = 0;
        inline static bool arena_created = false;
};

template <> inline bool render_manager::manager<render_manager>::initialized = false;

/*!
 * Gets passed the entity manager and timer then draws everything to screen.
 */
inline void render_manager::render(const menu_manager& menus, const entity_manager& world) {
    //  Clear screen to black.
    al_set_target_backbuffer(al_get_current_display());
    al_clear_to_color(WTE_COLOR_BLACK);

    /*
     * Calculate fps.
     */
    fps_counter++;
    //  Update fps on unique ticks only.
    const bool queue_not_empty = al_get_next_event(fps_event_queue, &fps_event);
    if(fps_event.type == ALLEGRO_EVENT_TIMER && queue_not_empty) {
        fps = fps_counter;
        fps_counter = 0;
        al_set_timer_count(fps_timer, 0);
    }
    
    /*
     * Render world if the game is running.
     */
    if(engine_flags::is_set(GAME_STARTED)) {
        //  Set drawing to the arena bitmap.
        al_set_target_bitmap(arena_bmp);
        al_clear_to_color(WTE_COLOR_BLACK);

        /*
         * Draw the backgrounds.
         */
        const component_container background_components = world.get_components<cmp::background>();

        //  Sort the background layers.
        std::set<entity_component_pair, render_comparator> background_componenet_set(
            background_components.begin(), background_components.end(), comparator);

        //  Draw each background by layer.
        for(ec_pair_citerator it = background_componenet_set.begin(); it != background_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible)
                al_draw_bitmap(world.get_component<cmp::background>(it->first)->background_bitmap, 0, 0, 0);
        }

        /*
         * Draw the sprites.
         */
        const component_container sprite_components = world.get_components<cmp::sprite>();

        //  Sort the sprite render components.
        std::set<entity_component_pair, render_comparator> sprite_componenet_set(
            sprite_components.begin(), sprite_components.end(), comparator);

        //  Draw each sprite in order.
        for(ec_pair_citerator it = sprite_componenet_set.begin(); it != sprite_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible) {
                al_draw_bitmap_region(world.get_component<cmp::sprite>(it->first)->sprite_bitmap,
                    world.get_component<cmp::sprite>(it->first)->sprite_x, world.get_component<cmp::sprite>(it->first)->sprite_y,
                    world.get_component<cmp::sprite>(it->first)->sprite_width, world.get_component<cmp::sprite>(it->first)->sprite_height,
                    world.get_component<cmp::location>(it->first)->pos_x + world.get_component<cmp::sprite>(it->first)->draw_offset_x,
                    world.get_component<cmp::location>(it->first)->pos_y + world.get_component<cmp::sprite>(it->first)->draw_offset_y,
                    0);
            }
        }

        /*
         * Draw hitboxes if enabled.
         * Use different colors for each team.
         * Note:  Re-uses sprite container for rendering.
         */
        #if WTE_DEBUG_MODE == 3 || WTE_DEBUG_MODE == 9
        for(ec_pair_citerator it = sprite_componenet_set.begin(); it != sprite_componenet_set.end(); it++) {
            //  Make sure the entity has a hitbox and is enabled.
            if((world.has_component<cmp::hitbox>(it->first)) &&
                (world.get_component<cmp::enabled>(it->first)->is_enabled)) {
                //  Select color based on team.
                ALLEGRO_COLOR team_color;
                switch(world.get_component<cmp::team>(it->first)->this_team) {
                    case 0: team_color = WTE_COLOR_GREEN; break;
                    case 1: team_color = WTE_COLOR_RED; break;
                    case 2: team_color = WTE_COLOR_BLUE; break;
                    default: team_color = WTE_COLOR_YELLOW;
                }
                //  Draw the hitbox.
                for(int i = 0; i < world.get_component<cmp::hitbox>(it->first)->width; i++) {
                    for(int j = 0; j < world.get_component<cmp::hitbox>(it->first)->height; j++) {
                        al_draw_pixel(world.get_component<cmp::location>(it->first)->pos_x + i,
                                        world.get_component<cmp::location>(it->first)->pos_y + j,
                                        team_color);
                    }
                }  //  End hitbox drawing.
            }  //  End hitbox/enabled test.
        }  //  End render component loop.
        #endif  //  End draw hitbox check.

        /*
         * Draw the overlays.
         */
        const component_container overlay_components = world.get_components<cmp::overlay>();

        //  Sort the overlay layers.
        std::set<entity_component_pair, render_comparator> overlay_componenet_set(
            overlay_components.begin(), overlay_components.end(), comparator);

        //  Draw each overlay by layer.
        for(ec_pair_citerator it = overlay_componenet_set.begin(); it != overlay_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible)
                al_draw_bitmap(world.get_component<cmp::overlay>(it->first)->overlay_bitmap,
                               world.get_component<cmp::overlay>(it->first)->pos_x,
                               world.get_component<cmp::overlay>(it->first)->pos_y, 0);
        }

        //  Draw the arena bitmap to the screen.
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(arena_bmp, 0, 0, arena_w, arena_h,
                              (screen_w / 2) - (arena_w * scale_factor / 2),
                              (screen_h / 2) - (arena_h * scale_factor / 2),
                              arena_w * scale_factor, arena_h * scale_factor, 0);
    } else {
        /*
         * Game is not running - draw the title screen.
         */
        al_set_target_backbuffer(al_get_current_display());
        //if(!title_bmp) al_clear_to_color(WTE_COLOR_BLACK);
        /*else*/ al_draw_scaled_bitmap(title_bmp, 0, 0,
                                   al_get_bitmap_width(title_bmp), al_get_bitmap_height(title_bmp),
                                   0, 0, screen_w, screen_h, 0);
    }

    /*
     * Render game menu if it's opened.
     */
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
        render_tmp_bmp = al_clone_bitmap(menus.render_menu());
        al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);

        al_set_target_backbuffer(al_get_current_display());
        al_draw_bitmap(render_tmp_bmp,
                       (screen_w / 2) - (al_get_bitmap_width(render_tmp_bmp) / 2),
                       (screen_h / 2) - (al_get_bitmap_height(render_tmp_bmp) / 2),
                       0);
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
     * Render alerts.
     */
    if(alert::is_set()) {
        int font_size = al_get_font_line_height(overlay_font);

        al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
        render_tmp_bmp = al_create_bitmap((alert::get_alert().length() * font_size) + 20, font_size + 20);
        al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);

        al_set_target_bitmap(render_tmp_bmp);
        al_clear_to_color(alert::get_bg_color());

        al_draw_text(overlay_font, alert::get_font_color(),
                     (al_get_bitmap_width(render_tmp_bmp) / 2), 10,
                     ALLEGRO_ALIGN_CENTER, alert::get_alert().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_bitmap(render_tmp_bmp,
                       (screen_w / 2) - (al_get_bitmap_width(render_tmp_bmp) / 2),
                       (screen_h / 2) - (al_get_bitmap_height(render_tmp_bmp) / 2),
                       0);
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
     * Framerate and timer rendering.
     */
    //  Draw frame rate.
    if(engine_flags::is_set(DRAW_FPS)) {
        std::string fps_string = "FPS: " + std::to_string(fps);
        al_draw_text(overlay_font, WTE_COLOR_YELLOW, screen_w, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }

    //  Draw time if debug mode is enabled.
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    std::string timer_string = "Timer: " + std::to_string(check_time());
    al_draw_text(overlay_font, WTE_COLOR_YELLOW, screen_w, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
     * Update the screen
     */
    al_flip_display();
}

} //  namespace mgr

} //  namespace wte

#endif
