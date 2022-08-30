/*!
 * wtengine | File:  display.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.1
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/display.hpp"

namespace wte {

std::string display::window_title = "WtEngine";
bool display::initialized = false;
ALLEGRO_DISPLAY* display::_display;

/*
 *
 */
display::display() {
    if(initialized == true) throw std::runtime_error("Display instance already running!");
    initialized = true;
}

/*
 *
 */
void display::set_window_title(const std::string& title) { window_title = title; }

/*
 *
 */
void display::set_display_mode(const std::size_t& m) {
    config::_gfx::display_mode = m;
    config::_gfx::needs_reconfig = true;
}

/*
 *
 */
void display::set_scale_factor(const float& f) {
    config::_gfx::scale_factor = f;
    config::_gfx::needs_reconfig = true;
}

/*
 *
 */
void display::create_display(void) {
    al_reset_new_display_options();

    //  Configure vsync options.  Gfx driver may override this.
    //  0 - System decides | 1 - VSync on | 2 - VSync off
    if(config::gfx::vsync >= 0 || config::gfx::vsync <= 2) {
        al_set_new_display_option(ALLEGRO_VSYNC, config::gfx::vsync, ALLEGRO_SUGGEST);
    } else {
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
    }

    int screen_w = config::gfx::arena_w;
    int screen_h = config::gfx::arena_h;

    //  Check if a display mode is set.
    if(config::gfx::display_mode == 1) {
        (opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_FULLSCREEN_WINDOW):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN_WINDOW));
    } else {
        (opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
        screen_w = (int)ceil(config::gfx::arena_w * config::gfx::scale_factor);
        screen_h = (int)ceil(config::gfx::arena_h * config::gfx::scale_factor);
    }

    //  Create the display.  Full screen windowed defaults to the display resolution.
    _display = al_create_display(screen_w, screen_h);
    if(config::gfx::display_mode == 1) {
        screen_w = al_get_display_width(_display);
        screen_h = al_get_display_height(_display);
    }

    //  Display failed to load, try a fallback.
    if(!_display) {
        (opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
        _display = al_create_display(config::gfx::arena_w,
                                     config::gfx::arena_h);
        if(!_display) throw std::runtime_error("Failed to configure display!");
        config::_gfx::display_mode = 0;
        config::_gfx::scale_factor = 1.0f;
        screen_w = config::gfx::arena_w;
        screen_h = config::gfx::arena_h;
    }

    //  Set window title.
    al_set_window_title(_display, window_title.c_str());

    //  Set window icon.
    ALLEGRO_FILE* file;
    file = al_fopen("icon.bmp", "rb");
    if(file) {
        ALLEGRO_BITMAP* icon_bitmap = al_load_bitmap_f(file, ".bmp");
        al_set_display_icon(_display, icon_bitmap);
        al_destroy_bitmap(icon_bitmap);
    }
    al_fclose(file);

    config::_gfx::screen_w = screen_w;
    config::_gfx::screen_h = screen_h;
}

/*
 *
 */
void display::destroy_display(void) { al_destroy_display(_display); }

/*
 *
 */
void display::reconf_display(void) {
    al_bitmap_converter::backup_bitmaps();
    destroy_display();
    create_display();
    al_convert_memory_bitmaps();
    al_bitmap_converter::reload_bitmaps();
    config::_gfx::needs_reconfig = false;
}

/*
 *
 */
void display::resize_display(const int& width, const int& height) {
    al_resize_display(_display, width, height);
    if(!al_acknowledge_resize(_display))
        throw std::runtime_error("Failed to resize display!");
}

}  //  end namespace wte
