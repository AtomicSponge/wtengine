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

#include <wte_demo.hpp>

int main(int argc, char **argv) {
    //  Set locations to load game data from.
    wte::engine::add_file_location("data.zip");

    //  Set the title screen.
    wte::mgr::gfx::renderer::set_title_screen("title.bmp");

    //  Set the size of the play area.
    wte::mgr::gfx::renderer::set_arena_size(768, 1024);

    //  Load settings.
    wte::config::load();

    //  Initialize game object.
    wte_demo my_game(argc, argv);
    //  Run the game loop.
    my_game.do_game();

    //  Save settings.
    wte::config::save();

    return 0; //  Exit program.
}
