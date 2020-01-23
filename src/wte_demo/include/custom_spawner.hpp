/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_spawner.hpp

  See LICENSE.txt for copyright information

  Header file for custom spawner handler
*/

#ifndef WTEDEMO_CUSTOM_SPAWNER_HPP
#define WTEDEMO_CUSTOM_SPAWNER_HPP

#include "wte/wte.hpp"

class custom_spawner : public wte::sys::spawner {
    private:
        void process_spawn(wte::mgr::entity_manager&, wte::msg::arg_list);

        void new_asteroid(wte::mgr::entity_manager&, float, float, float, float, int);
        void new_wall(wte::mgr::entity_manager&, float, float);
};

#endif
