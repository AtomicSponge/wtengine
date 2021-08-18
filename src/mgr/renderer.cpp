/*!
 * WTEngine | File:  renderer.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/renderer.hpp"

namespace wte
{

namespace mgr
{

template <> bool renderer::manager<renderer>::initialized = false;

ALLEGRO_BITMAP* renderer::temp_bitmap = NULL;
ALLEGRO_TIMER* renderer::fps_timer = NULL;
ALLEGRO_EVENT_QUEUE* renderer::fps_event_queue = NULL;
ALLEGRO_EVENT renderer::fps_event;
wte_asset<al_bitmap> renderer::arena_bitmap;
wte_asset<al_bitmap> renderer::title_bitmap;
wte_asset<al_font> renderer::renderer_font;
std::size_t renderer::fps_counter = 0, renderer::fps = 0;
int renderer::screen_w = 0, renderer::screen_h = 0;
int renderer::arena_w = 0, renderer::arena_h = 0;
bool renderer::arena_created = false;
std::string renderer::title_screen_file;
std::string renderer::background_file;

/*
 *
 */
void renderer::initialize(void) {
    //  Create the arena bitmap.
    if(arena_w == 0 || arena_h == 0) throw std::runtime_error("Arena size not defined!");
    arena_bitmap = make_asset(al_bitmap(arena_w, arena_h, true));
    //  Add reference to Asset manager so bitmap can be reloaded.
    mgr::assets<al_bitmap>::load<al_bitmap>("wte_renderer_arena_bitmap", arena_bitmap);
    arena_created = true;

    //  Set the overlay's font to the system default.
    renderer_font = mgr::assets<al_font>::get<al_font>("wte_default_font");

    //  Load the title screen bitmap.
    if(title_screen_file.empty()) {
        title_bitmap = make_asset(al_bitmap(1, 1));
        al_set_target_bitmap(**title_bitmap);
        al_clear_to_color(WTE_COLOR_BLACK);
    } else {
        title_bitmap = make_asset(al_bitmap());
        title_bitmap->load(title_screen_file);
    }

    fps_timer = al_create_timer(1);
    fps_event_queue = al_create_event_queue();
    al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));
    al_start_timer(fps_timer);
}

/*
 *
 */
void renderer::de_init(void) {
    al_destroy_event_queue(fps_event_queue);
    al_destroy_timer(fps_timer);
}

/*
 *
 */
void renderer::set_resolution(const int& w, const int& h) {
    screen_w = w;
    screen_h = h;
}

/*
 *
 */
void renderer::set_arena_size(const int& w, const int& h) {
    assert(w > 0 && h > 0);
    if(!arena_created) {
        arena_w = w;
        arena_h = h;
    }
}

/*
 *
 */
const int renderer::get_arena_width(void) { return arena_w; }

/*
 *
 */
const int renderer::get_arena_height(void) { return arena_h; }

/*
 *
 */
void renderer::set_title_screen(const std::string& fname) { title_screen_file = fname; }

/*
 *
 */
void renderer::set_font(wte_asset<al_font> font) { renderer_font = font; }

/*
 *
 */
void renderer::render(void) {
    float scale_factor = config::gfx::scale_factor;
    /*
     * Calculate fps.
     */
    fps_counter++;
    //  Update fps on unique ticks only.
    const bool queue_not_empty = al_get_next_event(fps_event_queue, &fps_event);
    if(queue_not_empty && fps_event.type == ALLEGRO_EVENT_TIMER) {
        fps = fps_counter;
        fps_counter = 0;
        al_set_timer_count(fps_timer, 0);
    }

    //  Toggle no preserve texture for faster rendering.
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

    //  Set drawing to the screen.
    al_set_target_backbuffer(al_get_current_display());
    al_clear_to_color(WTE_COLOR_BLACK);

    /*
     * Render world if the game is running.
     */
    if(config::flags::game_started) {
        //  Set drawing to the arena bitmap.
        al_set_target_bitmap(**arena_bitmap);
        al_clear_to_color(WTE_COLOR_BLACK);

        /*
         * Draw the backgrounds.
         */
        const const_component_container<cmp::background> background_components =
            mgr::world::get_components<cmp::background>();

        //  Sort the background layers.
        std::multiset<entity_component_pair<cmp::background>,
            comparator<entity_component_pair<cmp::background>>> background_componenet_set(
            background_components.begin(), background_components.end());

        //  Draw each background by layer.
        for(auto & it : background_componenet_set) {
            if(it.second->is_visible()) {
                if(it.second->draw_tinted())
                    al_draw_tinted_bitmap(
                        it.second->get_bitmap(),
                        it.second->get_tint(),
                        it.second->get_pos_x(),
                        it.second->get_pos_y(), 0
                    );
                else
                    al_draw_bitmap(
                        it.second->get_bitmap(),
                        it.second->get_pos_x(),
                        it.second->get_pos_y(), 0
                    );
            }
        }

        /*
         * Draw the sprites.
         */
        const const_component_container<cmp::sprite> sprite_components =
            mgr::world::get_components<cmp::sprite>();

        //  Sort the sprite components.
        std::multiset<entity_component_pair<cmp::sprite>,
            comparator<entity_component_pair<cmp::sprite>>> sprite_componenet_set(
            sprite_components.begin(), sprite_components.end());

        //  Draw each sprite in order.
        for(auto & it : sprite_componenet_set) {
            try {
                if(it.second->is_visible()) {
                    //  Get the current sprite frame.
                    temp_bitmap = al_create_sub_bitmap(
                        it.second->get_bitmap(),
                        it.second->get_sprite_x(),
                        it.second->get_sprite_y(),
                        it.second->get_sprite_width(),
                        it.second->get_sprite_height()
                    );

                    float sprite_angle = 0.0f;
                    float center_x = 0.0f, center_y = 0.0f;
                    float destination_x = 0.0f, destination_y = 0.0f;

                    try {
                        cmp::const_comp_ptr<cmp::location> temp_get = mgr::world::get_component<cmp::location>(it.first);
                        //  Check if the sprite should be rotated.
                        if(it.second->draw_rotated()) {
                            sprite_angle = it.second->get_direction();
                            center_x = (al_get_bitmap_width(temp_bitmap) / 2);
                            center_y = (al_get_bitmap_height(temp_bitmap) / 2);

                            destination_x = temp_get->get_x() +
                                (al_get_bitmap_width(temp_bitmap) * it.second->get_scale_factor_x() / 2) +
                                (it.second->get_draw_offset_x() * it.second->get_scale_factor_x());
                            destination_y = temp_get->get_y() +
                                (al_get_bitmap_height(temp_bitmap) * it.second->get_scale_factor_y() / 2) +
                                (it.second->get_draw_offset_y() * it.second->get_scale_factor_y());
                        } else {
                                destination_x = temp_get->get_x() + it.second->get_draw_offset_x();
                                destination_y = temp_get->get_y() + it.second->get_draw_offset_y();
                        }
                    } catch(...) { /* Didn't have location component */ }

                    //  Draw the sprite.
                    if(it.second->draw_tinted())
                        al_draw_tinted_scaled_rotated_bitmap(
                            temp_bitmap, it.second->get_tint(),
                            center_x, center_y, destination_x, destination_y,
                            it.second->get_scale_factor_x(),
                            it.second->get_scale_factor_y(),
                            sprite_angle, 0
                        );
                    else
                        al_draw_scaled_rotated_bitmap(
                            temp_bitmap, center_x, center_y, destination_x, destination_y,
                            it.second->get_scale_factor_x(),
                            it.second->get_scale_factor_y(),
                            sprite_angle, 0
                        );
                    al_destroy_bitmap(temp_bitmap);
                }
            } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when(), true); }
        }

        #if WTE_DEBUG_MODE
        /*
         * Draw sprite hitboxes if enabled.
         * Use different colors for each team.
         * Note:  Re-uses sprite container for rendering.
         */
        /*for(auto & it : sprite_componenet_set) {
            try {
                //  Make sure the entity is enabled.
                if(mgr::world::get_component<cmp::enabled>(it.first)->check()) {
                    //  Select color based on team.
                    ALLEGRO_COLOR team_color;
                    switch(mgr::world::get_component<cmp::team>(it.first)->get_team()) {
                        case 0: team_color = WTE_COLOR_GREEN; break;
                        case 1: team_color = WTE_COLOR_RED; break;
                        case 2: team_color = WTE_COLOR_BLUE; break;
                        default: team_color = WTE_COLOR_YELLOW;
                    }
                    //  Draw the hitbox.
                    temp_bitmap = al_create_bitmap(mgr::world::get_component<cmp::hitbox>(it.first)->get_width(),
                                                    mgr::world::get_component<cmp::hitbox>(it.first)->get_height());
                    al_set_target_bitmap(temp_bitmap);
                    al_clear_to_color(team_color);
                    al_set_target_bitmap(**arena_bitmap);
                    al_draw_bitmap(temp_bitmap,
                                mgr::world::get_component<cmp::location>(it.first)->get_x(),
                                mgr::world::get_component<cmp::location>(it.first)->get_y(), 0);
                    al_destroy_bitmap(temp_bitmap);
                }  //  End hitbox/enabled test.
            } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when(), true); }
        }  //  End render component loop.*/
        #endif  //  End draw hitbox check.

        /*
         * Draw the overlays.
         */
        const const_component_container<cmp::overlay> overlay_components =
            mgr::world::get_components<cmp::overlay>();

        //  Sort the overlay layers.
        std::multiset<entity_component_pair<cmp::overlay>,
            comparator<entity_component_pair<cmp::overlay>>> overlay_componenet_set(
            overlay_components.begin(), overlay_components.end());

        //  Draw each overlay by layer.
        for(auto & it : overlay_componenet_set) {
            if(it.second->is_visible()) {
                if(it.second->draw_tinted())
                    al_draw_tinted_bitmap(
                        it.second->get_bitmap(), it.second->get_tint(),
                        it.second->get_pos_x(), it.second->get_pos_y(), 0
                    );
                else
                    al_draw_bitmap(
                        it.second->get_bitmap(),
                        it.second->get_pos_x(), it.second->get_pos_y(), 0
                    );
            }
        }

        /*
         * Draw the arena bitmap to the screen.
         */
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(**arena_bitmap, 0, 0, arena_w, arena_h,
                              (screen_w / 2) - (arena_w * scale_factor / 2),
                              (screen_h / 2) - (arena_h * scale_factor / 2),
                              arena_w * scale_factor, arena_h * scale_factor, 0);
    } else {
        /*
         * Game is not running - draw the title screen.
         */
        al_draw_scaled_bitmap(**title_bitmap, 0, 0,
                              title_bitmap->get_width(), title_bitmap->get_height(),
                              0, 0, screen_w, screen_h, 0);
    }

    /*
     * Render game menu if it's opened.
     */
    if(config::flags::game_menu_opened) {
        temp_bitmap = al_clone_bitmap(mgr::menus::render_menu());
        al_set_target_backbuffer(al_get_current_display());

        al_draw_scaled_bitmap(
            temp_bitmap, 0, 0,
            al_get_bitmap_width(temp_bitmap), al_get_bitmap_height(temp_bitmap),
            (screen_w / 2) - std::floor((al_get_bitmap_width(temp_bitmap) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(temp_bitmap) * scale_factor) / 2),
            al_get_bitmap_width(temp_bitmap) * scale_factor,
            al_get_bitmap_height(temp_bitmap) * scale_factor, 0
        );
        al_destroy_bitmap(temp_bitmap);
    }

    /*
     * Render alerts.
     */
    if(alert::is_set()) {
        int font_size = al_get_font_line_height(**renderer_font);

        temp_bitmap = al_create_bitmap((alert::get().length() * font_size) + 20, font_size + 20);
        al_set_target_bitmap(temp_bitmap);
        al_clear_to_color(alert::get_bg_color());

        al_draw_text(**renderer_font, alert::get_font_color(),
                     (al_get_bitmap_width(temp_bitmap) / 2), 10,
                     ALLEGRO_ALIGN_CENTER, alert::get().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            temp_bitmap, 0, 0,
            al_get_bitmap_width(temp_bitmap), al_get_bitmap_height(temp_bitmap),
            (screen_w / 2) - std::floor((al_get_bitmap_width(temp_bitmap) * scale_factor) / 2),
            (screen_h / 2) - std::floor((al_get_bitmap_height(temp_bitmap) * scale_factor) / 2),
            al_get_bitmap_width(temp_bitmap) * scale_factor,
            al_get_bitmap_height(temp_bitmap) * scale_factor, 0
        );
        al_destroy_bitmap(temp_bitmap);
    }

    /*
     * Framerate and timer rendering.
     */
    //  Draw frame rate.
    if(config::flags::draw_fps) {
        std::string fps_string = "FPS: " + std::to_string(fps);
        al_draw_text(**renderer_font, WTE_COLOR_YELLOW, screen_w, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }

    //  Draw time if debug mode is enabled.
    #if WTE_DEBUG_MODE
    std::string timer_string = "Timer: " + std::to_string(engine_time::check_time());
    al_draw_text(**renderer_font, WTE_COLOR_YELLOW, screen_w, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
     * Update the screen
     */
    al_flip_display();
}

} //  namespace mgr

} //  namespace wte
