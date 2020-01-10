/*
  WTEngine Demo
  By:  Matthew Evans
  File:  game_loop.hpp

  See LICENSE.txt for copyright information

  Header file for game loop object
*/

#ifndef WTEDEMO_GAME_LOOP_HPP
#define WTEDEMO_GAME_LOOP_HPP

#include "wte/wte.hpp"

class game_loop : public wte::wte_main {
    private:
    //  Overrides from wte_main:
    void load_menus(void);
    void load_systems(void);
    void load_game(void);
    void end_game(void);
};

#endif
