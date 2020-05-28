/*
  WTEngine Demo
  By:  Matthew Evans
  File:  main.cpp

  See LICENSE.txt for copyright information

  Main file for WTEngine demo
*/

#include <wte/wte.hpp>

#include "include/wte_demo.hpp"

using namespace wte;

int main(int argc, char **argv) {
    //  Set locations to load game data from.
    wte_main::add_file_location("data");
    wte_main::add_file_location("data.zip");

    //  Register default engine settings
    engine_cfg::reg("screen_width=768");
    engine_cfg::reg("screen_height=1024");

    //  Load settings
    engine_cfg_map::load();

    //  Initialize game object
    wte_demo my_game(argc, argv);
    //  Run the game loop
    my_game.do_game();

    //  Save settings
    engine_cfg_map::save();

    return 0; //  Exit program
}
