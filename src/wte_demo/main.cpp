/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  main.cpp
 *
 * See LICENSE.txt for copyright information
 *
 * Main file for WTEngine demo
 */

//  Set gamepad input mode to digital.
#define WTE_INPUT_MODE (0)

#include <wte/wte.hpp>

#include "include/wte_demo.hpp"

using namespace wte;

int main(int argc, char **argv) {
    //  Set locations to load game data from.
    wte_main::add_file_location("data.zip");

    //  Set the title screen.
    mgr::render_manager::set_title_screen("title.bmp");
    //mgr::render_manager::set_background_screen("title.bmp");

    //  Set the size of the play area.
    mgr::render_manager::set_arena_size(768, 1024);
    //mgr::render_manager::set_arena_size(1920, 1080);
    //mgr::render_manager::set_arena_size(1440, 1080);

    //  Register default engine settings.
    engine_cfg::reg("display_mode=windowed");
    engine_cfg::reg("scale_factor=1");
    engine_cfg::reg("draw_fps=0");

    //  Load settings.
    engine_cfg::load();

    //  Initialize game object.
    wte_demo my_game(argc, argv);
    //  Run the game loop.
    my_game.do_game();

    //  Save settings.
    engine_cfg::save();

    return 0; //  Exit program.
}
