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

#define _USE_MATH_DEFINES

#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cmath>
#include <stdexcept>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "manager.hpp"
#include "engine_time.hpp"

#include "../wte_global_defines.hpp"
#include "../engine_flags.hpp"
#include "../alert.hpp"
#include "../cmp/components.hpp"
#include "menu_manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace mgr
{

//!  Container for an entity and component pair.  Used for sorting.
typedef std::pair<entity, cmp::component_csptr> entity_component_pair;
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
            //  Define render comparator as lambda function that sorts components.
            comparator =
                [](const entity_component_pair r_element1, const entity_component_pair r_element2) {
                    return r_element1.second < r_element2.second;
                };
        };

        /*!
         * render_manager destructor.
         * \param void
         * \return void
         */
        inline ~render_manager() {};

        /*!
         * \brief Initialize the render manager.
         * Configures the internal objects for the render manager to use.
         * \param void
         * \return void
         */
        inline void initialize(void) {
            if(arena_w == 0 || arena_h == 0) throw std::runtime_error("Arena size not defined!");
            al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);
            arena_bmp = al_create_bitmap(arena_w, arena_h);
            al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
            arena_created = true;

            if(title_screen_file.empty()) {
                title_bmp = al_create_bitmap(1, 1);
                al_set_target_bitmap(title_bmp);
                al_clear_to_color(WTE_COLOR_BLACK);
            } else {
                ALLEGRO_FILE* file;
                file = al_fopen(title_screen_file.c_str(), "rb");
                if(!file) {
                    title_bmp = al_create_bitmap(1, 1);
                    al_set_target_bitmap(title_bmp);
                    al_clear_to_color(WTE_COLOR_BLACK);
                } else {
                    title_bmp = al_load_bitmap_f(file, title_screen_file.substr(title_screen_file.find("."),
                                                 title_screen_file.length()).c_str());
                }
                al_fclose(file);
            }

            if(render_font_file.empty()) overlay_font = al_create_builtin_font();
            else {
                overlay_font = al_load_font(render_font_file.c_str(), render_font_size, render_font_flags);
                if(!overlay_font) overlay_font = al_create_builtin_font();
            }
            if(!overlay_font) throw std::runtime_error("Unable to set font for renderer!");

            fps_timer = al_create_timer(1);
            fps_event_queue = al_create_event_queue();
            al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));
            al_start_timer(fps_timer);
        };

        /*!
         * \brief De-initialize the render manager.
         * Destories the internal objects.
         * \param void
         * \return void
         */
        inline void de_init(void) {
            al_destroy_bitmap(title_bmp);
            al_destroy_bitmap(arena_bmp);

            al_destroy_font(overlay_font);

            al_destroy_event_queue(fps_event_queue);
            al_destroy_timer(fps_timer);
        };

        /*!
         * \brief Inform the renderer of the screen resolution.
         * Gets called by the engine when the screen resolution is updated.
         * \param w Resolution width in pixels.
         * \param h Resolution height in pixels.
         * \return void
         */
        inline void update_resolution(const int w, const int h) {
            screen_w = w;
            screen_h = h;
        };

        /*!
         * \brief Set scale factor.
         * This is used to scale the game arena and menus to a larger size.
         * For support of higher resolution screens.
         * \param f New scale factor value.
         * \return void
         */
        inline void set_scale_factor(const float f) {
            scale_factor = f;
        };

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
         * \brief Set the title screen.
         * This should be called during engine initialization before the main object is created.
         * \param fname Filename of the title screen.
         * \return void
         */
        inline static void set_title_screen(const std::string fname) {
            title_screen_file = fname;
        };

        /*!
         * \brief Set the font to be used by the renderer.
         * If not called, Allegro's default font will be used.
         * This should be called during engine initialization before the main object is created.
         * See the Allegro docs for more information on al_load_font and the parameters used.
         * \param fname Filename to use.
         * \param size Font size.
         * \param flags Font flags.
         * \return void
         */
        inline static void set_font_file(const std::string fname, const int size, const int flags) {
            render_font_file = fname;
            render_font_size = size;
            render_font_flags = flags;
        };

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

        inline static std::string title_screen_file = "";
        inline static std::string render_font_file = "";
        inline static int render_font_size = 0;
        inline static int render_font_flags = 0;
};

template <> inline bool render_manager::manager<render_manager>::initialized = false;

/*!
 * \brief Render method - Draw the game screen.
 * Gets passed the entity manager and timer then draws everything to screen.
 * \param menus Reference to menu manager.
 * \param world Reference to entity manager.
 * \return void
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

    //  Toggle no preserve texture for faster rendering.
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

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
        const const_component_container background_components = world.get_components<cmp::background>();

        //  Sort the background layers.
        std::set<entity_component_pair, render_comparator> background_componenet_set(
            background_components.begin(), background_components.end(), comparator);

        //  Draw each background by layer.
        /*for(ec_pair_citerator it = background_componenet_set.begin(); it != background_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible)
                al_draw_bitmap(static_cast<cmp::background*>(it->second.get())->background_bitmap, 0, 0, 0);
        }*/

        /*
         * Draw the sprites.
         */
        const const_component_container sprite_components = world.get_components<cmp::sprite>();

        //  Sort the sprite render components.
        std::set<entity_component_pair, render_comparator> sprite_componenet_set(
            sprite_components.begin(), sprite_components.end(), comparator);

        //  Draw each sprite in order.
        /*for(ec_pair_citerator it = sprite_componenet_set.begin(); it != sprite_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible) {
                //  Get the current sprite frame.
                render_tmp_bmp = al_create_sub_bitmap(
                    static_cast<cmp::sprite*>(it->second.get())->sprite_bitmap,
                    static_cast<cmp::sprite*>(it->second.get())->sprite_x,
                    static_cast<cmp::sprite*>(it->second.get())->sprite_y,
                    static_cast<cmp::sprite*>(it->second.get())->sprite_width,
                    static_cast<cmp::sprite*>(it->second.get())->sprite_height
                );

                float sprite_angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                //  Check if the sprite should be rotated.
                if(world.has_component<cmp::direction>(it->first)) {
                    if(world.get_component<cmp::direction>(it->first)->draw_rotated) {
                        sprite_angle = world.get_component<cmp::direction>(it->first)->angle * (M_PI / 180);
                        center_x = (al_get_bitmap_width(render_tmp_bmp) / 2);
                        center_y = (al_get_bitmap_height(render_tmp_bmp) / 2);

                        destination_x = world.get_component<cmp::location>(it->first)->pos_x +
                            (al_get_bitmap_width(render_tmp_bmp) / 2) +
                            (static_cast<cmp::sprite*>(it->second.get())->draw_offset_x *
                            static_cast<cmp::sprite*>(it->second.get())->scale_factor_x);
                        destination_y = world.get_component<cmp::location>(it->first)->pos_y +
                            (al_get_bitmap_height(render_tmp_bmp) / 2) +
                            (static_cast<cmp::sprite*>(it->second.get())->draw_offset_y *
                            static_cast<cmp::sprite*>(it->second.get())->scale_factor_y);
                    }
                } else {
                    destination_x = world.get_component<cmp::location>(it->first)->pos_x +
                        static_cast<cmp::sprite*>(it->second.get())->draw_offset_x;
                    destination_y = world.get_component<cmp::location>(it->first)->pos_y +
                        static_cast<cmp::sprite*>(it->second.get())->draw_offset_y;
                }
                //  Draw the sprite.
                al_draw_scaled_rotated_bitmap(
                    render_tmp_bmp, center_x, center_y, destination_x, destination_y,
                    static_cast<cmp::sprite*>(it->second.get())->scale_factor_x,
                    static_cast<cmp::sprite*>(it->second.get())->scale_factor_y,
                    sprite_angle, 0
                );
                al_destroy_bitmap(render_tmp_bmp);
            }
        }*/

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
        const const_component_container overlay_components = world.get_components<cmp::overlay>();

        //  Sort the overlay layers.
        std::set<entity_component_pair, render_comparator> overlay_componenet_set(
            overlay_components.begin(), overlay_components.end(), comparator);

        //  Draw each overlay by layer.
        /*for(ec_pair_citerator it = overlay_componenet_set.begin(); it != overlay_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible)
                al_draw_bitmap(static_cast<cmp::overlay*>(it->second.get())->overlay_bitmap,
                               static_cast<cmp::overlay*>(it->second.get())->pos_x,
                               static_cast<cmp::overlay*>(it->second.get())->pos_y, 0);
        }*/

        /*
         * Draw the arena bitmap to the screen.
         */
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
        al_draw_scaled_bitmap(title_bmp, 0, 0,
                              al_get_bitmap_width(title_bmp), al_get_bitmap_height(title_bmp),
                              0, 0, screen_w, screen_h, 0);
    }

    /*
     * Render game menu if it's opened.
     */
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        render_tmp_bmp = al_clone_bitmap(menus.render_menu());
        al_set_target_backbuffer(al_get_current_display());

        al_draw_scaled_bitmap(
            render_tmp_bmp, 0, 0,
            al_get_bitmap_width(render_tmp_bmp), al_get_bitmap_height(render_tmp_bmp),
            (screen_w / 2) - std::floor((al_get_bitmap_width(render_tmp_bmp) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(render_tmp_bmp) * scale_factor) / 2),
            al_get_bitmap_width(render_tmp_bmp) * scale_factor,
            al_get_bitmap_height(render_tmp_bmp) * scale_factor, 0
        );
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
     * Render alerts.
     */
    if(alert::is_set()) {
        int font_size = al_get_font_line_height(overlay_font);

        render_tmp_bmp = al_create_bitmap((alert::get_alert().length() * font_size) + 20, font_size + 20);
        al_set_target_bitmap(render_tmp_bmp);
        al_clear_to_color(alert::get_bg_color());

        al_draw_text(overlay_font, alert::get_font_color(),
                     (al_get_bitmap_width(render_tmp_bmp) / 2), 10,
                     ALLEGRO_ALIGN_CENTER, alert::get_alert().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            render_tmp_bmp, 0, 0,
            al_get_bitmap_width(render_tmp_bmp), al_get_bitmap_height(render_tmp_bmp),
            (screen_w / 2) - std::floor((al_get_bitmap_width(render_tmp_bmp) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(render_tmp_bmp) * scale_factor) / 2),
            al_get_bitmap_width(render_tmp_bmp) * scale_factor,
            al_get_bitmap_height(render_tmp_bmp) * scale_factor, 0
        );
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
    //  Set default bitmap flags.
    al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);
}

} //  namespace mgr

} //  namespace wte

#endif
