/*!
 * WTEngine | File:  wte_display.hpp
 * 
 * \author Matthew Evans
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 */

#include <string>
#include <stdexcept>
#include <cmath>

#include <allegro5/allegro.h>

#include "wtengine/wte_display.hpp"

namespace wte
{

const std::string wte_display::get_window_title(void) const {
    return window_title;
}

wte_display::wte_display(const std::string& title) : window_title(title) {};

void wte_display::create_display(void) {
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

    int screen_w = mgr::render_manager::get_arena_width();
    int screen_h = mgr::render_manager::get_arena_height();

    //  Check if a display mode is set.
    if(!engine_cfg::is_reg("display_mode")) engine_cfg::reg("display_mode=windowed");
    if(engine_cfg::get("display_mode") == "windowed_full_screen") {
        al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
    } else {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        screen_w = (int)ceil(mgr::render_manager::get_arena_width() * scale_factor);
        screen_h = (int)ceil(mgr::render_manager::get_arena_height() * scale_factor);
    }

    //  Create the display.  Full screen windowed defaults to the display resolution.
    display = al_create_display(screen_w, screen_h);

    //  Display failed to load, try a fallback.
    if(!display) {
        al_set_new_display_flags(ALLEGRO_WINDOWED);
        display = al_create_display(mgr::render_manager::get_arena_width(),
                                    mgr::render_manager::get_arena_height());
        if(!display) throw std::runtime_error("Failed to configure display!");
        engine_cfg::set("display_mode=windowed");
        engine_cfg::set("scale_factor=1");
        scale_factor = 1.0f;
        screen_w = mgr::render_manager::get_arena_width();
        screen_h = mgr::render_manager::get_arena_height();
    }

    //  Set window title.
    al_set_window_title(display, window_title.c_str());

    //  Set window icon.
    ALLEGRO_FILE* file;
    file = al_fopen("icon.bmp", "rb");
    if(file) {
        ALLEGRO_BITMAP* icon_bitmap = al_load_bitmap_f(file, ".bmp");
        al_set_display_icon(display, icon_bitmap);
        al_destroy_bitmap(icon_bitmap);
    }
    al_fclose(file);

    /* *** Render manager updating *** */
    if(engine_cfg::get("display_mode") == "windowed_full_screen") {
        screen_w = al_get_display_width(display);
        screen_h = al_get_display_height(display);
    }
    screen.update_resolution(screen_w, screen_h);
    screen.set_scale_factor(scale_factor);
}

void wte_display::destroy_display(void) {
    al_destroy_display(display);
}

void wte_display::reconf_display(void) {
    destroy_display();
    create_display();
    al_convert_memory_bitmaps();
}

} //  end namespace wte
