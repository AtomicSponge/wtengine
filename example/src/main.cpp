/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  main.cpp
 *
 * See LICENSE.md for copyright information
 *
 * Main file for WTEngine demo
 * Initializes engine and starts the game loop.
 */

#include <wtengine/wtengine.hpp>

#include "include/wte_demo.hpp"

using namespace wte;

int main(int argc, char **argv) {
    //  Set locations to load game data from.
    engine::add_file_location("data.zip");

    //  Set the title screen.
    mgr::renderer::set_title_screen("title.bmp");
    //mgr::renderer::set_background_screen("title.bmp");

    //  Set the size of the play area.
    mgr::renderer::set_arena_size(768, 1024);
    //mgr::renderer::set_arena_size(1920, 1080);
    //mgr::renderer::set_arena_size(1440, 1080);

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
