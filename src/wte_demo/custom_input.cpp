/*
 * WTEngine Demo
 * By:  Matthew Evans
 * File:  custom_input.cpp
 *
 * See LICENSE.txt for copyright information
 *
 * Custom input code
 * This overrides the input system to define custom behavior
 */

#include "include/custom_input.hpp"

using namespace wte;

/*
 * Custom input code
 */
void custom_input::custom_run(mgr::entity_manager& world, mgr::message_manager& messages) {
    entity player_entity;
    entity cannon_entity;
    entity shield_entity;

    for(component_iterator it = input_components.begin(); it != input_components.end(); it++) {
        if(world.get_component<cmp::name>(it->first)->name_str == "player") player_entity = it->first;
        if(world.get_component<cmp::name>(it->first)->name_str == "main_cannon") cannon_entity = it->first;
        if(world.get_component<cmp::name>(it->first)->name_str == "shield") shield_entity = it->first;
    }

    if(input_flags::is_set(INPUT_UP)) {
        if(world.get_component<cmp::location>(player_entity)->pos_y > 0)
            world.set_component<cmp::location>(player_entity)->pos_y -= 5;
    }
    if(input_flags::is_set(INPUT_DOWN)) {
        if(world.get_component<cmp::location>(player_entity)->pos_y < mgr::render_manager::get_arena_height() - 32)
            world.set_component<cmp::location>(player_entity)->pos_y += 5;
    }
    if(input_flags::is_set(INPUT_LEFT)) {
        if(world.get_component<cmp::location>(player_entity)->pos_x > 12)
            world.set_component<cmp::location>(player_entity)->pos_x -= 5;
    }
    if(input_flags::is_set(INPUT_RIGHT)) {
        if(world.get_component<cmp::location>(player_entity)->pos_x < mgr::render_manager::get_arena_width() - 22)
            world.set_component<cmp::location>(player_entity)->pos_x += 5;
    }
    /*if(input_flags::is_set(INPUT_LEFT)) {
        if(world.get_component<cmp::direction>(player_entity)->angle < 0)
            world.set_component<cmp::direction>(player_entity)->angle = 360;
        world.set_component<cmp::direction>(player_entity)->angle -= 5;
    }
    if(input_flags::is_set(INPUT_RIGHT)) {
        if(world.get_component<cmp::direction>(player_entity)->angle > 360)
            world.set_component<cmp::direction>(player_entity)->angle = 0;
        world.set_component<cmp::direction>(player_entity)->angle += 5;
    }*/

    if(input_flags::is_set(INPUT_ACTION_1)) {
        //  Set the cannon's location to match the player
        world.set_component<cmp::location>(cannon_entity)->pos_x =
            world.get_component<cmp::location>(player_entity)->pos_x;
        world.set_component<cmp::location>(cannon_entity)->pos_y =
            world.get_component<cmp::location>(player_entity)->pos_y -
            world.get_component<cmp::hitbox>(cannon_entity)->height;

        world.set_component<cmp::visible>(cannon_entity)->is_visible = true;
        world.set_component<cmp::enabled>(cannon_entity)->is_enabled = true;
    }
    if(!input_flags::is_set(INPUT_ACTION_1) && world.get_component<cmp::enabled>(cannon_entity)->is_enabled) {
        world.set_component<cmp::visible>(cannon_entity)->is_visible = false;
        world.set_component<cmp::enabled>(cannon_entity)->is_enabled = false;
    }

    if(input_flags::is_set(INPUT_ACTION_2) && game_cfg::get<int>("shield") > 0) {
        //  Set the shield's location to match the player
        world.set_component<cmp::location>(shield_entity)->pos_x =
            world.get_component<cmp::location>(player_entity)->pos_x - 28;
        world.set_component<cmp::location>(shield_entity)->pos_y =
            world.get_component<cmp::location>(player_entity)->pos_y - 16;

        world.set_component<cmp::visible>(shield_entity)->is_visible = true;
        world.set_component<cmp::enabled>(shield_entity)->is_enabled = true;
        world.set_component<cmp::hitbox>(player_entity)->solid = false;
        game_cfg_map::subtract<int>("shield", 1);
    } else {
        if(game_cfg::get<int>("shield") < game_cfg::get<int>("max_shield"))
            game_cfg_map::add<int>("shield", 1);
    }
    if((!input_flags::is_set(INPUT_ACTION_2) && world.get_component<cmp::enabled>(shield_entity)->is_enabled)
        || game_cfg::get<int>("shield") == 0) {
        world.set_component<cmp::visible>(shield_entity)->is_visible = false;
        world.set_component<cmp::enabled>(shield_entity)->is_enabled = false;
        world.set_component<cmp::hitbox>(player_entity)->solid = true;
    }
}
