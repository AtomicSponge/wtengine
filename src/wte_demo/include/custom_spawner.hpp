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

class custom_spawner : public wte::ecs::sys::spawner {
    private:
        void process_spawns(wte::ecs::entity_manager&, wte::msg::message_container);

        void new_asteroid(wte::ecs::entity_manager&, float, float, float, float, int);
        void new_wall(wte::ecs::entity_manager&, float, float);
};

#endif
