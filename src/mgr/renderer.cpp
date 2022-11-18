/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#include "wtengine/mgr/renderer.hpp"

template <> bool wte::mgr::manager<wte::mgr::gfx::renderer>::initialized = false;

namespace wte::mgr::gfx {

ALLEGRO_TIMER* renderer::fps_timer = NULL;
ALLEGRO_EVENT_QUEUE* renderer::fps_event_queue = NULL;
ALLEGRO_EVENT renderer::fps_event;
wte_asset<ALLEGRO_BITMAP> renderer::viewport_bitmap = nullptr;
wte_asset<ALLEGRO_BITMAP> renderer::title_bitmap = nullptr;
wte_asset<ALLEGRO_FONT> renderer::renderer_font = nullptr;
std::size_t renderer::fps_counter = 0, renderer::_fps = 0;
time_point<system_clock> renderer::_last_render;
time_point<system_clock> renderer::_start_time;
duration renderer::_delta_time;
bool renderer::arena_created = false;
std::string renderer::title_screen_file;
std::string renderer::background_file;

const std::size_t& renderer::fps = renderer::_fps;
const time_point<system_clock>& renderer::last_render = renderer::_last_render;
const time_point<system_clock>& renderer::start_time = renderer::_start_time;
const duration& renderer::delta_time = renderer::_delta_time;

/*
 *
 */
void renderer::initialize(void) {
    //  Check that screen and arena sizes were defined.
    if(config::gfx::screen_w == 0 || config::gfx::screen_h == 0) throw std::runtime_error("Screen size not defined!");
    if(config::gfx::viewport_w == 0 || config::gfx::viewport_h == 0) throw std::runtime_error("Arena size not defined!");
    //  Create the arena bitmap.
    viewport_bitmap = make_asset<ALLEGRO_BITMAP>(config::gfx::viewport_w, config::gfx::viewport_h);
    arena_created = true;

    //  Set the overlay's font to the system default.
    renderer_font = mgr::assets::get<ALLEGRO_FONT>("wte_default_font");

    //  Load the title screen bitmap.
    if(title_screen_file.empty()) {
        title_bitmap = make_asset<ALLEGRO_BITMAP>(1, 1);
        al_set_target_bitmap(title_bitmap.get());
        al_clear_to_color(al_map_rgb(0,0,0));
    } else {
        title_bitmap = make_asset<ALLEGRO_BITMAP>(title_screen_file);
    }

    fps_timer = al_create_timer(1);
    fps_event_queue = al_create_event_queue();
    al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));
    al_start_timer(fps_timer);

    _start_time = _last_render = system_clock::now();
    _delta_time = system_clock::now() - system_clock::now();
}

/*
 *
 */
void renderer::de_init(void) {
    viewport_bitmap.reset();
    title_bitmap.reset();
    renderer_font.reset();
    al_destroy_event_queue(fps_event_queue);
    al_destroy_timer(fps_timer);
}

/*
 *
 */
void renderer::set_viewport_size(const int& w, const int& h) {
    assert(w > 0 && h > 0);
    if(!arena_created) {
        config::_gfx::viewport_w = w;
        config::_gfx::viewport_h = h;
    }
}

/*
 *
 */
void renderer::set_title_screen(const std::string& fname) { title_screen_file = fname; }

/*
 *
 */
void renderer::set_font(wte_asset<ALLEGRO_FONT> font) { renderer_font = font; }

/*
 *
 */
void renderer::draw_hitboxes(void) {
    const const_component_container<cmp::hitbox> hitbox_components =
        mgr::world::get_components<cmp::hitbox>();

    for(auto& it: hitbox_components) {
        if(it.second->solid) {
            //  Select color based on team.
            ALLEGRO_COLOR team_color;
            switch(it.second->team) {
                case 0: team_color = al_map_rgb(0,255,0); break;
                case 1: team_color = al_map_rgb(255,0,0); break;
                case 2: team_color = al_map_rgb(0,0,255); break;
                default: team_color = al_map_rgb(255,255,0);
            }
            //  Draw the hitbox.
            ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap(
                it.second->width,
                it.second->height);
            al_set_target_bitmap(temp_bitmap);
            al_clear_to_color(team_color);
            al_set_target_bitmap(viewport_bitmap.get());
            try {
                al_draw_bitmap(temp_bitmap,
                    mgr::world::get_component<cmp::location>(it.first)->pos_x,
                    mgr::world::get_component<cmp::location>(it.first)->pos_y, 0);
            } catch(const std::exception& e) { throw e; }
            al_destroy_bitmap(temp_bitmap);
        }
    }
}

/*
 *
 */
void renderer::draw_timer(void) {
    if constexpr (build_options.debug_mode) {
        const std::string timer_string = "Timer: " + std::to_string(engine_time::check());
        al_draw_text(renderer_font.get(), al_map_rgb(255,255,0), config::gfx::screen_w, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    }
}

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
        _fps = fps_counter;
        fps_counter = 0;
        al_set_timer_count(fps_timer, 0);
    }

    //  Toggle no preserve texture for faster rendering.
    al_set_new_bitmap_flags(ALLEGRO_NO_PRESERVE_TEXTURE);

    //  Set drawing to the screen.
    al_set_target_backbuffer(al_get_current_display());
    al_clear_to_color(al_map_rgb(0,0,0));

    //  Render world if the game is running.
    if(config::flags::engine_started) {
        //  Set drawing to the arena bitmap.
        al_set_target_bitmap(viewport_bitmap.get());
        al_clear_to_color(al_map_rgb(0,0,0));

        //  Draw the backgrounds.
        const const_component_container<cmp::gfx::background> background_components =
            mgr::world::get_components<cmp::gfx::background>();

        //  Sort the background layers.
        std::multiset<entity_component_pair<cmp::gfx::background>,
            comparator<entity_component_pair<cmp::gfx::background>>> background_componenet_set(
            background_components.begin(), background_components.end());

        //  Draw each background by layer.
        for(auto& it: background_componenet_set) {
            if(it.second->visible) {
                float angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                if(it.second->rotated) {
                    angle = it.second->direction;
                    center_x = (al_get_bitmap_width(it.second->_bitmap.get()) / 2);
                    center_y = (al_get_bitmap_height(it.second->_bitmap.get()) / 2);

                    destination_x = it.second->pos_x +
                        (al_get_bitmap_width(it.second->_bitmap.get()) * it.second->scale_factor_x / 2);
                    destination_y = it.second->pos_y +
                        (al_get_bitmap_height(it.second->_bitmap.get()) * it.second->scale_factor_y / 2);
                } else {
                        destination_x = it.second->pos_x;
                        destination_y = it.second->pos_y;
                }

                if(it.second->tinted)
                    al_draw_tinted_scaled_rotated_bitmap(
                        it.second->_bitmap.get(), it.second->get_tint(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
                else
                    al_draw_scaled_rotated_bitmap(
                        it.second->_bitmap.get(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
            }
        }

        //  Draw the sprites.
        const const_component_container<cmp::gfx::sprite> sprite_components =
            mgr::world::get_components<cmp::gfx::sprite>();

        //  Sort the sprite components.
        std::multiset<entity_component_pair<cmp::gfx::sprite>,
            comparator<entity_component_pair<cmp::gfx::sprite>>> sprite_componenet_set(
            sprite_components.begin(), sprite_components.end());

        //  Draw each sprite in order.
        for(auto& it: sprite_componenet_set) {
            if(it.second->visible) {
                //  Get the current sprite frame.
                ALLEGRO_BITMAP* temp_bitmap = al_create_sub_bitmap(
                    it.second->_bitmap.get(),
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
                    if(it.second->tinted)
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

        //  Draw hitboxes if debug is enabled.
        if constexpr (build_options.debug_mode)
            if(config::flags::show_hitboxes) draw_hitboxes();

        //  Draw the overlays.
        const const_component_container<cmp::gfx::overlay> overlay_components =
            mgr::world::get_components<cmp::gfx::overlay>();

        //  Sort the overlay layers.
        std::multiset<entity_component_pair<cmp::gfx::overlay>,
            comparator<entity_component_pair<cmp::gfx::overlay>>> overlay_componenet_set(
            overlay_components.begin(), overlay_components.end());

        //  Draw each overlay by layer.
        for(auto& it: overlay_componenet_set) {
            if(it.second->visible) {
                float angle = 0.0f;
                float center_x = 0.0f, center_y = 0.0f;
                float destination_x = 0.0f, destination_y = 0.0f;

                if(it.second->rotated) {
                    angle = it.second->direction;
                    center_x = (al_get_bitmap_width(it.second->_bitmap.get()) / 2);
                    center_y = (al_get_bitmap_height(it.second->_bitmap.get()) / 2);

                    destination_x = it.second->pos_x +
                        (al_get_bitmap_width(it.second->_bitmap.get()) * it.second->scale_factor_x / 2);
                    destination_y = it.second->pos_y +
                        (al_get_bitmap_height(it.second->_bitmap.get()) * it.second->scale_factor_y / 2);
                } else {
                        destination_x = it.second->pos_x;
                        destination_y = it.second->pos_y;
                }

                if(it.second->tinted)
                    al_draw_tinted_scaled_rotated_bitmap(
                        it.second->_bitmap.get(), it.second->get_tint(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
                else
                    al_draw_scaled_rotated_bitmap(
                        it.second->_bitmap.get(),
                        center_x, center_y, destination_x, destination_y,
                        it.second->scale_factor_x,
                        it.second->scale_factor_y,
                        angle, 0
                    );
            }
        }

        //  Draw the viewport bitmap to the screen.
        al_set_target_backbuffer(al_get_current_display());
        al_draw_scaled_bitmap(
            viewport_bitmap.get(), 0, 0, config::gfx::viewport_w, config::gfx::viewport_h,
            (config::gfx::screen_w / 2) - (config::gfx::viewport_w * config::gfx::scale_factor / 2),
            (config::gfx::screen_h / 2) - (config::gfx::viewport_h * config::gfx::scale_factor / 2),
            config::gfx::viewport_w * config::gfx::scale_factor,
            config::gfx::viewport_h * config::gfx::scale_factor, 0);
    } else {  //  Game is not running
        //  Draw the title screen.
        al_draw_scaled_bitmap(title_bitmap.get(), 0, 0,
            al_get_bitmap_width(title_bitmap.get()), al_get_bitmap_height(title_bitmap.get()),
            (config::gfx::screen_w / 2) - (al_get_bitmap_width(title_bitmap.get()) * config::gfx::scale_factor / 2),
            (config::gfx::screen_h / 2) - (al_get_bitmap_height(title_bitmap.get()) * config::gfx::scale_factor / 2),
            al_get_bitmap_width(title_bitmap.get()) * config::gfx::scale_factor,
            al_get_bitmap_height(title_bitmap.get()) * config::gfx::scale_factor, 0);
    }

    //  Draw frame rate.
    if(config::flags::draw_fps) {
        const std::string fps_string = "FPS: " + std::to_string(fps);
        al_draw_text(renderer_font.get(), al_map_rgb(255,255,0), config::gfx::screen_w, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }
    if constexpr (build_options.debug_mode) draw_timer();

    //  Render Dear ImGui
    if constexpr (build_options.use_imgui) draw_gui();
    
    //  Update the screen & delta time.
    al_flip_display();
    _delta_time = system_clock::now() - _last_render;
    _last_render = system_clock::now();
}

}  //  namespace wte::mgr
