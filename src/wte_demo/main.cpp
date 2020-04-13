/*
  WTEngine Demo
  By:  Matthew Evans
  File:  main.cpp

  See LICENSE.txt for copyright information

  Main file for WTEngine demo
*/

#include "include/wte/wte.hpp"
#include "include/wte_demo.hpp"

using namespace wte;

int main(int argc, char **argv) {
    //  Register engine settings
    engine_cfg::reg("screen_width=768");
    engine_cfg::reg("screen_height=1024");

    //  Load settings
    engine_cfg_map::load();

    //game_cfg::reg("test=1");

    //  Initialize game object
    wte_demo my_game;
    my_game.wte_load();

    //  Run the game loop
    my_game.do_game();

    //  Clean up
    my_game.wte_unload();

    //  Save settings
    engine_cfg_map::save();

    return 0; //  Exit program
}
