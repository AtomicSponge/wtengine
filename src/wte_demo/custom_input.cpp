/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_input.cpp

  See LICENSE.txt for copyright information

  Custom input code
  This overrides the input system to define custom behavior
*/

#include "include/custom_input.hpp"

/*
  Custom input code
*/
void custom_input::custom_run(wte::mgr::entity_manager& world, wte::mgr::message_manager& messages) {
    wte::entity player_entity;
    wte::entity cannon_entity;
    wte::entity shield_entity;

    for(wte::component_iterator it = input_components.begin(); it != input_components.end(); it++) {
        if(world.get_component<wte::cmp::name>(it->first)->name_str == "player") player_entity = it->first;
        if(world.get_component<wte::cmp::name>(it->first)->name_str == "main_cannon") cannon_entity = it->first;
        if(world.get_component<wte::cmp::name>(it->first)->name_str == "shield") shield_entity = it->first;
    }

    if(wte::key_flags::is_set(wte::KEY_UP)) {
        if(world.get_component<wte::cmp::location>(player_entity)->pos_y > 0)
            world.set_component<wte::cmp::location>(player_entity)->pos_y -= 5;
    }
    if(wte::key_flags::is_set(wte::KEY_DOWN)) {
        if(world.get_component<wte::cmp::location>(player_entity)->pos_y < wte::wte_config::screen_height)
            world.set_component<wte::cmp::location>(player_entity)->pos_y += 5;
    }
    if(wte::key_flags::is_set(wte::KEY_LEFT)) {
        if(world.get_component<wte::cmp::location>(player_entity)->pos_x > 0)
            world.set_component<wte::cmp::location>(player_entity)->pos_x -= 5;
    }
    if(wte::key_flags::is_set(wte::KEY_RIGHT)) {
        if(world.get_component<wte::cmp::location>(player_entity)->pos_x < wte::wte_config::screen_width - 10)
            world.set_component<wte::cmp::location>(player_entity)->pos_x += 5;
    }

    if(wte::key_flags::is_set(wte::KEY_FIRE_1)) {
        //  Set the cannon's location to match the player
        world.set_component<wte::cmp::location>(cannon_entity)->pos_x =
            world.get_component<wte::cmp::location>(player_entity)->pos_x;
        world.set_component<wte::cmp::location>(cannon_entity)->pos_y =
            world.get_component<wte::cmp::location>(player_entity)->pos_y -
            world.get_component<wte::cmp::hitbox>(cannon_entity)->height;

        world.set_component<wte::cmp::visible>(cannon_entity)->is_visible = true;
        world.set_component<wte::cmp::enabled>(cannon_entity)->is_enabled = true;
    } else {
        world.set_component<wte::cmp::visible>(cannon_entity)->is_visible = false;
        world.set_component<wte::cmp::enabled>(cannon_entity)->is_enabled = false;
    }

    if(wte::key_flags::is_set(wte::KEY_FIRE_2)) {
        //  Set the shield's location to match the player
        world.set_component<wte::cmp::location>(shield_entity)->pos_x =
            world.get_component<wte::cmp::location>(player_entity)->pos_x -
            (world.get_component<wte::cmp::hitbox>(shield_entity)->width / 2) +
            (world.get_component<wte::cmp::hitbox>(player_entity)->width / 2);
        world.set_component<wte::cmp::location>(shield_entity)->pos_y =
            world.get_component<wte::cmp::location>(player_entity)->pos_y -
            (world.get_component<wte::cmp::hitbox>(shield_entity)->height / 2) +
            (world.get_component<wte::cmp::hitbox>(player_entity)->height / 2);

        world.set_component<wte::cmp::visible>(shield_entity)->is_visible = true;
        world.set_component<wte::cmp::enabled>(shield_entity)->is_enabled = true;
        world.set_component<wte::cmp::hitbox>(player_entity)->solid = false;
    } else {
        world.set_component<wte::cmp::visible>(shield_entity)->is_visible = false;
        world.set_component<wte::cmp::enabled>(shield_entity)->is_enabled = false;
        world.set_component<wte::cmp::hitbox>(player_entity)->solid = true;
    }
}
