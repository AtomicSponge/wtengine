/*
  WTEngine Demo
  By:  Matthew Evans
  File:  main.cpp

  See LICENSE.txt for copyright information

  Main file for WTEngine demo
*/

#include "include/wte/wte.hpp"

#include "include/main.hpp"
#include "include/game_loop.hpp"

//  Initialize game flags
volatile bool wte::key[WTE_NUM_KEY_FLAGS] = { false, false, false, false, false, false, false };
volatile bool wte::game_flag[WTE_NUM_GAME_FLAGS] = { false, false, false, false, false };

//  Initialize global message queue for audio
wte::audio_deck wte::audio_messages;

int main(int argc, char **argv) {
    int init_test;

    //  Set some default values for the game flags
    wte::game_flag[wte::IS_RUNNING] = true;
    wte::game_flag[wte::GAME_STARTED] = false;
    wte::game_flag[wte::GAME_MENU_OPENED] = false;
    wte::game_flag[wte::DRAW_HITBOX] = true;
    wte::game_flag[wte::DRAW_FPS] = true;

    //  Initialize game object
    game_loop main_game_loop = game_loop();

    init_test = main_game_loop.wte_init();
    if(init_test != 0) {
        return init_test; //  WTE Init failed, exit
    }

    //  Run the game loop
    main_game_loop.do_game();

    //  Clean up
    main_game_loop.wte_unload();

    return 0; //  Exit program
}
