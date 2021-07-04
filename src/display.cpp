/*!
 * WTEngine | File:  display.cpp
 * 
 * \author Matthew Evans
 * \version 0.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/display.hpp"

namespace wte
{

/*
 *
 */
const std::string display::get_window_title(void) const { return window_title; }

/*
 *
 */
display::display(const std::string& title) : window_title(title) {}

/*
 *
 */
void display::create_display(void) {
    al_reset_new_display_options();

    //  Configure vsync options.  Gfx driver may override this.
    //  0 - System decides | 1 - VSync on | 2 - VSync off
    if(!engine_cfg::is_reg("vsync")) engine_cfg::reg("vsync=0");
    if(engine_cfg::get<int>("vsync") >= 0 || engine_cfg::get<int>("vsync") <= 2) {
        al_set_new_display_option(ALLEGRO_VSYNC, engine_cfg::get<int>("vsync"), ALLEGRO_SUGGEST);
    } else {
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
        engine_cfg::set("vsync=0");
    }

    //  Set the scale factor.
    float scale_factor = 1.0f;
    if(!engine_cfg::is_reg("scale_factor")) engine_cfg::reg("scale_factor=1");
    if(engine_cfg::get("scale_factor") == "0.5") scale_factor = 0.5f;
    else if(engine_cfg::get("scale_factor") == "1") scale_factor = 1.0f;
    else if(engine_cfg::get("scale_factor") == "1.25") scale_factor = 1.25f;
    else if(engine_cfg::get("scale_factor") == "1.5") scale_factor = 1.5f;
    else if(engine_cfg::get("scale_factor") == "1.75") scale_factor = 1.75f;
    else if(engine_cfg::get("scale_factor") == "2") scale_factor = 2.0f;
    else {
        scale_factor = 1.0f;
        engine_cfg::set("scale_factor=1");
    }

    int screen_w = mgr::renderer::get_arena_width();
    int screen_h = mgr::renderer::get_arena_height();

    //  Check if a display mode is set.
    if(!engine_cfg::is_reg("display_mode")) engine_cfg::reg("display_mode=windowed");
    if(engine_cfg::get("display_mode") == "windowed_full_screen") {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    } else {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        screen_w = (int)ceil(mgr::renderer::get_arena_width() * scale_factor);
        screen_h = (int)ceil(mgr::renderer::get_arena_height() * scale_factor);
    }

    //  Create the display.  Full screen windowed defaults to the display resolution.
    _display = al_create_display(screen_w, screen_h);

    //  Display failed to load, try a fallback.
    if(!_display) {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        _display = al_create_display(mgr::renderer::get_arena_width(),
                                     mgr::renderer::get_arena_height());
        if(!_display) throw std::runtime_error("Failed to configure display!");
        engine_cfg::set("display_mode=windowed");
        engine_cfg::set("scale_factor=1");
        scale_factor = 1.0f;
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
    if(engine_cfg::get("display_mode") == "windowed_full_screen") {
        screen_w = al_get_display_width(_display);
        screen_h = al_get_display_height(_display);
    }
    mgr::renderer::update_resolution(screen_w, screen_h);
    mgr::renderer::set_scale_factor(scale_factor);
}

/*
 *
 */
void display::destroy_display(void) { al_destroy_display(_display); }

/*
 *
 */
void display::reconf_display(void) {
    destroy_display();
    create_display();
    al_convert_memory_bitmaps();
}

} //  end namespace wte
