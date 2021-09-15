/*!
 * WTEngine | File:  renderer.cpp
 * 
 * \author Matthew Evans
 * \version 0.7
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/renderer.hpp"

namespace wte::mgr {

template <> bool renderer::manager<renderer>::initialized = false;

ALLEGRO_TIMER* renderer::fps_timer = NULL;
ALLEGRO_EVENT_QUEUE* renderer::fps_event_queue = NULL;
ALLEGRO_EVENT renderer::fps_event;
wte_asset<al_bitmap> renderer::arena_bitmap;
wte_asset<al_bitmap> renderer::title_bitmap;
wte_asset<al_bitmap> renderer::menu_bitmap;
wte_asset<al_font> renderer::renderer_font;
std::size_t renderer::fps_counter = 0, renderer::fps = 0;
bool renderer::arena_created = false;
std::string renderer::title_screen_file;
std::string renderer::background_file;

/*
 *
 */
void renderer::initialize(void) {
    //  Create the arena bitmap.
    if(config::gfx::arena_w == 0 || config::gfx::arena_h == 0) throw std::runtime_error("Arena size not defined!");
    arena_bitmap = make_asset(al_bitmap(config::gfx::arena_w, config::gfx::arena_h, true));
    //  Add reference to Asset manager so bitmap can be reloaded.
    mgr::assets<al_bitmap>::load<al_bitmap>("wte_renderer_arena_bitmap", arena_bitmap);
    arena_created = true;

    //  Set the overlay's font to the system default.
    renderer_font = mgr::assets<al_font>::get<al_font>("wte_default_font");
    //  Set the reference to the menu bitmap.
    menu_bitmap = mgr::assets<al_bitmap>::get<al_bitmap>("wte_menus_menu_buffer");

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
void renderer::set_arena_size(const int& w, const int& h) {
    assert(w > 0 && h > 0);
    if(!arena_created) {
        config::_gfx::arena_w = w;
        config::_gfx::arena_h = h;
    }
}

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
        for(auto& it: background_componenet_set) {
            if(it.second->visible) {
                float angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                if(it.second->rotated) {
                    angle = it.second->direction;
                    center_x = (al_get_bitmap_width(it.second->get_bitmap()) / 2);
                    center_y = (al_get_bitmap_height(it.second->get_bitmap()) / 2);

                    destination_x = it.second->pos_x +
                        (al_get_bitmap_width(it.second->get_bitmap()) * it.second->scale_factor_x / 2);
                    destination_y = it.second->pos_y +
                        (al_get_bitmap_height(it.second->get_bitmap()) * it.second->scale_factor_y / 2);
                } else {
                        destination_x = it.second->pos_x;
                        destination_y = it.second->pos_y;
                }

                if(it.second->draw_tinted())
                    al_draw_tinted_scaled_rotated_bitmap(
                        it.second->get_bitmap(), it.second->get_tint(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
                else
                    al_draw_scaled_rotated_bitmap(
                        it.second->get_bitmap(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
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
        for(auto& it: sprite_componenet_set) {
            if(it.second->visible) {
                //  Get the current sprite frame.
                ALLEGRO_BITMAP* temp_bitmap = al_create_sub_bitmap(
                    it.second->get_bitmap(),
                    it.second->sprite_x,
                    it.second->sprite_y,
                    it.second->sprite_width,
                    it.second->sprite_height
                );

                try {
                    float angle = 0.0f;
                    float center_x = 0.0f, center_y = 0.0f;
                    float destination_x = 0.0f, destination_y = 0.0f;
                    cmp::const_comp_ptr<cmp::location> temp_get = mgr::world::get_component<cmp::location>(it.first);

                    //  Check if the sprite should be rotated.
                    if(it.second->rotated) {
                        angle = it.second->direction;
                        center_x = (al_get_bitmap_width(temp_bitmap) / 2);
                        center_y = (al_get_bitmap_height(temp_bitmap) / 2);

                        destination_x = temp_get->pos_x +
                            (al_get_bitmap_width(temp_bitmap) * it.second->scale_factor_x / 2) +
                            (it.second->draw_offset_x * it.second->scale_factor_x);
                        destination_y = temp_get->pos_y +
                            (al_get_bitmap_height(temp_bitmap) * it.second->scale_factor_y / 2) +
                            (it.second->draw_offset_y * it.second->scale_factor_y);
                    } else {
                            destination_x = temp_get->pos_x + it.second->draw_offset_x;
                            destination_y = temp_get->pos_y + it.second->draw_offset_y;
                    }

                    //  Draw the sprite.
                    if(it.second->draw_tinted())
                        al_draw_tinted_scaled_rotated_bitmap(
                            temp_bitmap, it.second->get_tint(),
                            center_x, center_y, destination_x, destination_y,
                            it.second->scale_factor_x,
                            it.second->scale_factor_y,
                            angle, 0
                        );
                    else
                        al_draw_scaled_rotated_bitmap(
                            temp_bitmap, center_x, center_y, destination_x, destination_y,
                            it.second->scale_factor_x,
                            it.second->scale_factor_y,
                            angle, 0
                        );
                } catch(...) { throw; }

                al_destroy_bitmap(temp_bitmap);
            }
        }

        #if WTE_DEBUG_MODE
        /*
         * Draw hitboxes if enabled.  Use different colors for each team.
         */
        if(config::flags::show_hitboxes) {
            const const_component_container<cmp::hitbox> hitbox_components =
                mgr::world::get_components<cmp::hitbox>();
            
            for(auto& it: hitbox_components) {
                if(it.second->is_solid()) {
                    //  Select color based on team.
                    ALLEGRO_COLOR team_color;
                    try {
                        switch(mgr::world::get_component<cmp::team>(it.first)->get_team()) {
                            case 0: team_color = WTE_COLOR_GREEN; break;
                            case 1: team_color = WTE_COLOR_RED; break;
                            case 2: team_color = WTE_COLOR_BLUE; break;
                            default: team_color = WTE_COLOR_YELLOW;
                        }
                    } catch(...) { team_color = WTE_COLOR_RED; }
                    //  Draw the hitbox.
                    ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap(
                        it.second->get_width(),
                        it.second->get_height());
                    al_set_target_bitmap(temp_bitmap);
                    al_clear_to_color(team_color);
                    al_set_target_bitmap(**arena_bitmap);
                    try {
                        al_draw_bitmap(temp_bitmap,
                            mgr::world::get_component<cmp::location>(it.first)->get_x(),
                            mgr::world::get_component<cmp::location>(it.first)->get_y(), 0);
                    } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
                    al_destroy_bitmap(temp_bitmap);
                }
            }
        }
        #endif

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
        for(auto& it: overlay_componenet_set) {
            if(it.second->visible) {
                float angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                if(it.second->rotated) {
                    angle = it.second->direction;
                    center_x = (al_get_bitmap_width(it.second->get_bitmap()) / 2);
                    center_y = (al_get_bitmap_height(it.second->get_bitmap()) / 2);

                    destination_x = it.second->pos_x +
                        (al_get_bitmap_width(it.second->get_bitmap()) * it.second->scale_factor_x / 2);
                    destination_y = it.second->pos_y +
                        (al_get_bitmap_height(it.second->get_bitmap()) * it.second->scale_factor_y / 2);
                } else {
                        destination_x = it.second->pos_x;
                        destination_y = it.second->pos_y;
                }

                if(it.second->draw_tinted())
                    al_draw_tinted_scaled_rotated_bitmap(
                        it.second->get_bitmap(), it.second->get_tint(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
                else
                    al_draw_scaled_rotated_bitmap(
                        it.second->get_bitmap(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
            }
        }

        /*
         * Draw the arena bitmap to the screen.
         */
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            **arena_bitmap, 0, 0, config::gfx::arena_w, config::gfx::arena_h,
            (config::gfx::screen_w / 2) - (config::gfx::arena_w * config::gfx::scale_factor / 2),
            (config::gfx::screen_h / 2) - (config::gfx::arena_h * config::gfx::scale_factor / 2),
            config::gfx::arena_w * config::gfx::scale_factor,
            config::gfx::arena_h * config::gfx::scale_factor, 0);
    } else {
        /*
         * Game is not running - draw the title screen.
         */
        al_draw_scaled_bitmap(**title_bitmap, 0, 0,
            title_bitmap->get_width(), title_bitmap->get_height(),
            0, 0, config::gfx::screen_w, config::gfx::screen_h, 0);
    }

    const float scale_factor = config::gfx::menu_scale_factor * config::gfx::scale_factor;
    /*
     * Render game menu if it's opened.
     */
    if(config::flags::menu_opened) {
        mgr::menus::render_menu();
        const int menu_width = menu_bitmap->get_width();
        const int menu_height = menu_bitmap->get_height();
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            **menu_bitmap, 0, 0, menu_width, menu_height,
            (config::gfx::arena_w / 2) - (menu_width * scale_factor / 2),
            (config::gfx::arena_h / 2) - (menu_height * scale_factor / 2),
            menu_width * scale_factor,
            menu_height * scale_factor, 0
        );
    }

    /*
     * Render notices.
     */
    if(notice::is_set()) {
        const int font_size = al_get_font_line_height(**notice::get_notice_font());
        ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap((notice::get().length() * font_size) + (font_size * 2), font_size + (font_size * 2));
        al_set_target_bitmap(temp_bitmap);
        al_clear_to_color(notice::get_notice_bg_color());

        al_draw_text(**notice::get_notice_font(), notice::get_notice_font_color(),
            (al_get_bitmap_width(temp_bitmap) / 2), 10,
            ALLEGRO_ALIGN_CENTER, notice::get().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            temp_bitmap, 0, 0,
            al_get_bitmap_width(temp_bitmap), al_get_bitmap_height(temp_bitmap),
            (config::gfx::screen_w / 2) - std::floor((al_get_bitmap_width(temp_bitmap) * scale_factor) / 2),
            (config::gfx::screen_h / 2) - std::floor((al_get_bitmap_height(temp_bitmap) * scale_factor) / 2),
            al_get_bitmap_width(temp_bitmap) * scale_factor,
            al_get_bitmap_height(temp_bitmap) * scale_factor, 0
        );
        al_destroy_bitmap(temp_bitmap);
    }

    /*
     * Render alerts.
     */
    if(alert::is_set()) {
        const int font_size = al_get_font_line_height(**renderer_font);
        ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap((alert::get().length() * font_size) + (font_size * 2), font_size + (font_size * 2));
        al_set_target_bitmap(temp_bitmap);
        al_clear_to_color(WTE_COLOR_RED);

        al_draw_text(**renderer_font, WTE_COLOR_WHITE,
            (al_get_bitmap_width(temp_bitmap) / 2), 10,
            ALLEGRO_ALIGN_CENTER, alert::get().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            temp_bitmap, 0, 0,
            al_get_bitmap_width(temp_bitmap), al_get_bitmap_height(temp_bitmap),
            (config::gfx::screen_w / 2) - std::floor((al_get_bitmap_width(temp_bitmap) * scale_factor) / 2),
            (config::gfx::screen_h / 2) - std::floor((al_get_bitmap_height(temp_bitmap) * scale_factor) / 2),
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
        const std::string fps_string = "FPS: " + std::to_string(fps);
        al_draw_text(**renderer_font, WTE_COLOR_YELLOW, config::gfx::screen_w, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }

    //  Draw time if debug mode is enabled.
    #if WTE_DEBUG_MODE
    const std::string timer_string = "Timer: " + std::to_string(engine_time::check_time());
    al_draw_text(**renderer_font, WTE_COLOR_YELLOW, config::gfx::screen_w, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
     * Update the screen
     */
    al_flip_display();
}

}  //  namespace wte::mgr
