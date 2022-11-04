/*!
 * wtengine | File:  display.cpp
 * 
 * \author Matthew Evans
 * \version 0.8.0
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
    if(initialized == true) throw engine_error("Display instance already running!");
    initialized = true;
}

/*
 *
 */
void display::set_window_title(const std::string& title) { window_title = title; }

/*
 *
 */
void display::set_scale_factor(float f) {
    if(f < 0.5f || f > 2.0f) f = 1.0f;
    config::_gfx::scale_factor = f;
}

/*
 *
 */
void display::set_display_mode(std::size_t m) {
    if(m > 1) m = 1;
    config::_gfx::display_mode = m;
}

/*
 *
 */
void display::set_screen_size(int w, int h) {
    if(w < 1) w = 1;
    if(h < 1) h = 1;
    config::_gfx::screen_w = w;
    config::_gfx::screen_h = h;
}

/*
 *
 */
void display::resize_display(int w, int h) {
    if(w < 1) w = 1;
    if(h < 1) h = 1;
    config::_gfx::screen_w = w;
    config::_gfx::screen_h = h;
    al_resize_display(_display, config::gfx::screen_w, config::gfx::screen_h);
    if(!al_acknowledge_resize(_display))
        throw engine_error("Failed to resize display!");
}

/*
 *
 */
void display::set_vsync(std::size_t v) {
    if(v > 2) v = 0;
    config::_gfx::vsync = v;
}

/*
 *
 */
void display::create_display(void) {
    al_reset_new_display_options();

    //  Configure vsync options.  Gfx driver may override this.
    //  0 - System decides | 1 - VSync on | 2 - VSync off
    if(config::gfx::vsync >= 0 && config::gfx::vsync <= 2) {
        al_set_new_display_option(ALLEGRO_VSYNC, config::gfx::vsync, ALLEGRO_SUGGEST);
    } else {
        al_set_new_display_option(ALLEGRO_VSYNC, 0, ALLEGRO_SUGGEST);
    }

    //  Check if a display mode is set.
    if(config::gfx::display_mode == 1) {
        (build_options.opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_FULLSCREEN_WINDOW):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_FULLSCREEN_WINDOW));
    } else {
        (build_options.opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
    }

    //  Create the display.  Full screen windowed defaults to the display resolution.
    _display = al_create_display(config::gfx::screen_w, config::gfx::screen_h);

    //  Display failed to load, try a fallback.
    if(!_display) {
        (build_options.opengl_latest ?
            al_set_new_display_flags(ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED):
            al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_WINDOWED));
        _display = al_create_display(config::gfx::screen_w,
                                     config::gfx::screen_h);
        if(!_display) throw engine_error("Failed to configure display!");
        config::_gfx::display_mode = 0;
        config::_gfx::scale_factor = 1.0f;
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

    if constexpr (build_options.use_imgui) {
        //  Configure Dear ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if constexpr (build_options.imgui_darkmode) ImGui::StyleColorsDark();
        else ImGui::StyleColorsLight();
        ImGui_ImplAllegro5_Init(_display);
    }
}

/*
 *
 */
void display::destroy_display(void) { 
    if constexpr (build_options.use_imgui) {
        //  Shut down Dear ImGui
        ImGui_ImplAllegro5_Shutdown();
        ImGui::DestroyContext();
    }

    al_destroy_display(_display);
}

}  //  end namespace wte
