/*
  WTEngine Demo
  By:  Matthew Evans
  File:  starfield.cpp

  See LICENSE.txt for copyright information

  Custom system for rendering a starfield background
*/

#include "include/starfield.hpp"

using namespace wte;

/*
  Initialize the starfield
*/
starfield::starfield() : system("starfield"), speed_mult(1) {
    for(int i = 0; i < MAX_STARS; i++) {
        x[i] = std::rand() % engine_cfg::get<int>("screen_width") + 1;
        y[i] = std::rand() % engine_cfg::get<int>("screen_height") + 1;
        speed[i] = (std::rand() % 3 + 1) * 3;
        color[i] = std::rand() % 4 + 1;
    }
}

/*
  Update the starfield
*/
void starfield::run(mgr::entity_manager& world, mgr::message_manager& messages, int64_t current_time) {
    //  Find the background component and set drawing to it
    entity background_id = world.get_components<cmp::background>().begin()->first;
    al_set_target_bitmap(world.get_component<cmp::background>(background_id)->background_bitmap);

    //  Clear background to black
    al_clear_to_color(WTE_COLOR_BLACK);

    //  Move the stars
    for(int i = 0; i < MAX_STARS; i++) {
        y[i] += speed[i] * speed_mult;
        if(y[i] > engine_cfg::get<int>("screen_height")) { //  Make a new star
            x[i] = std::rand() % engine_cfg::get<int>("screen_width") + 1;
            y[i] = 0;
            speed[i] = (std::rand() % 3 + 1) * 3;
            color[i] = std::rand() % 4 + 1;
        }
    }

    //  Draw stars
    for(int i = 0; i < MAX_STARS; i++) {
        if(color[i] == 1 || 4) al_draw_pixel(x[i], y[i], WTE_COLOR_WHITE);
        if(color[i] == 2) al_draw_pixel(x[i], y[i], WTE_COLOR_YELLOW);
        if(color[i] == 3) al_draw_pixel(x[i], y[i], WTE_COLOR_RED);
    }
}

/*
  Process messages for the starfield
*/
void starfield::dispatch(mgr::entity_manager& world, message_container messages) {
    for(auto it = messages.begin(); it != messages.end(); it++) {
        if(it->get_cmd() == "default") speed_mult = 1;
        if(it->get_cmd() == "up") speed_mult *= 2;
        if(it->get_cmd() == "down") speed_mult /= 2;
        if(it->get_cmd() == "reset") {
            speed_mult = 1;

            for(int i = 0; i < MAX_STARS; i++) {
                x[i] = std::rand() % engine_cfg::get<int>("screen_width") + 1;
                y[i] = std::rand() % engine_cfg::get<int>("screen_height") + 1;
                speed[i] = (std::rand() % 3 + 1) * 3;
                color[i] = std::rand() % 4 + 1;
            }
        }
    }
}
