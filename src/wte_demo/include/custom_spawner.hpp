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

using namespace wte;

class custom_spawner : public sys::spawner {
    private:
        void process_spawn(mgr::entity_manager&, msg_arg_list);

        void new_asteroid(mgr::entity_manager&, float, float, float, float, int);
        void new_wall(mgr::entity_manager&, float, float);
};

#endif
