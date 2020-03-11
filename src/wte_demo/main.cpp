/*
  WTEngine Demo
  By:  Matthew Evans
  File:  main.cpp

  See LICENSE.txt for copyright information

  Main file for WTEngine demo
*/

#include "include/wte/wte.hpp"

//  Set game timer rate
const float wte::engine_cfg::TICKS_PER_SECOND = 60.0;

int main(int argc, char **argv) {
    wte::engine_cfg::screen_width = 768;
    wte::engine_cfg::screen_height = 1024;

    //  Initialize game object
    wte::wte_main my_game;
    my_game.wte_load();

    //  Run the game loop
    my_game.do_game();

    //  Clean up
    my_game.wte_unload();

    return 0; //  Exit program
}
