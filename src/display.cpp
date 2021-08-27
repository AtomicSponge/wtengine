/*!
 * WTEngine | File:  display.cpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/display.hpp"

namespace wte {

bool display::initialized = false;

ALLEGRO_DISPLAY* display::_display;

/*
 *
 */
display::display(const std::string& title) : window_title(title) {
    if(initialized == true) throw std::runtime_error("Display instance already running!");
    initialized = true;
}

/*
 *
 */
const std::string display::get_window_title(void) const { return window_title; }

/*
 *
 */
const int display::get_screen_width(void) { return al_get_display_width(_display); }

/*
 *
 */
const int display::get_screen_height(void) { return al_get_display_height(_display); }

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

    int screen_w = mgr::renderer::get_arena_width();
    int screen_h = mgr::renderer::get_arena_height();

    //  Check if a display mode is set.
    if(config::gfx::display_mode == 1) {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    } else {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        screen_w = (int)ceil(mgr::renderer::get_arena_width() * config::gfx::scale_factor);
        screen_h = (int)ceil(mgr::renderer::get_arena_height() * config::gfx::scale_factor);
    }

    //  Create the display.  Full screen windowed defaults to the display resolution.
    _display = al_create_display(screen_w, screen_h);
    if(config::gfx::display_mode == 1) {
        screen_w = al_get_display_width(_display);
        screen_h = al_get_display_height(_display);
    }

    //  Display failed to load, try a fallback.
    if(!_display) {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        _display = al_create_display(mgr::renderer::get_arena_width(),
                                     mgr::renderer::get_arena_height());
        if(!_display) throw std::runtime_error("Failed to configure display!");
        config::_gfx::display_mode = 0;
        config::_gfx::scale_factor = 1.0f;
        screen_w = mgr::renderer::get_arena_width();
        screen_h = mgr::renderer::get_arena_height();
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

    /* *** Render manager updating *** */
    mgr::renderer::set_resolution(screen_w, screen_h);
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
}

} //  end namespace wte
