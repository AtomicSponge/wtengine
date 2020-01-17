/*
  WTEngine Demo
  By:  Matthew Evans
  File:  game_loop.cpp

  See LICENSE.txt for copyright information

  The main game loop code
  Overrides wte_main
*/

#include "include/game_loop.hpp"
#include "include/custom_input.hpp"
#include "include/custom_spawner.hpp"
#include "include/custom_logic.hpp"
#include "include/starfield.hpp"

/*
  Load menus

  Overrides from wte_main
*/
void game_loop::load_menus(void) {
    menus.set_menu("main_menu")->set_title("WTE Demo");
    //wte::mnu::menu_item item;
    //menus.set_menu("game_menu")->add_item(item);
}

/*
  Load the systems in order

  Overrides from wte_main
*/
void game_loop::load_systems(void) {
    systems.add(std::make_unique<custom_input>());
    systems.add(std::make_unique<custom_spawner>());
    systems.add(std::make_unique<custom_logic>());
    systems.add(std::make_unique<wte::ecs::sys::colision>());
    systems.add(std::make_unique<wte::ecs::sys::animate>());
    systems.add(std::make_unique<starfield>());
}

/*
  Pre-load starting entities

  Overrides from wte_main
*/
void game_loop::load_game(void) {
    wte::ecs::entity e_id;

    //  Background entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::background>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::background_layer>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.set_component<wte::ecs::cmp::name>(e_id)->name = "starfield";
    world.set_component<wte::ecs::cmp::background_layer>(e_id)->layer = 0;
    world.set_component<wte::ecs::cmp::visible>(e_id)->is_visible = true;

    //  Player entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::health>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>());
    world.set_component<wte::ecs::cmp::name>(e_id)->name = "player";
    world.set_component<wte::ecs::cmp::team>(e_id)->team = 0;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_x = (wte::screen_width / 2) - 5;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_y = wte::screen_height - 40;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->width = 10;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->height = 10;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->solid = true;
    world.set_component<wte::ecs::cmp::health>(e_id)->hp = 1;
    world.set_component<wte::ecs::cmp::render_order>(e_id)->order = 1;
    world.set_component<wte::ecs::cmp::visible>(e_id)->is_visible = true;
    world.set_component<wte::ecs::cmp::enabled>(e_id)->is_enabled = true;

    //  Main cannon entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::damage>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>());
    world.set_component<wte::ecs::cmp::name>(e_id)->name = "main_cannon";
    world.set_component<wte::ecs::cmp::team>(e_id)->team = 0;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_x = 0;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_y = 0;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->width = 10;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->height = 200;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->solid = false;
    world.set_component<wte::ecs::cmp::damage>(e_id)->dmg = 5;
    world.set_component<wte::ecs::cmp::render_order>(e_id)->order = 2;
    world.set_component<wte::ecs::cmp::visible>(e_id)->is_visible = false;
    world.set_component<wte::ecs::cmp::enabled>(e_id)->is_enabled = false;

    //  Shield entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::damage>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>());
    world.set_component<wte::ecs::cmp::name>(e_id)->name = "shield";
    world.set_component<wte::ecs::cmp::team>(e_id)->team = 0;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_x = 0;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_y = 0;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->width = 40;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->height = 40;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->solid = false;
    world.set_component<wte::ecs::cmp::damage>(e_id)->dmg = 100;
    world.set_component<wte::ecs::cmp::render_order>(e_id)->order = 2;
    world.set_component<wte::ecs::cmp::visible>(e_id)->is_visible = false;
    world.set_component<wte::ecs::cmp::enabled>(e_id)->is_enabled = false;
}

/*
  Stop the game

  Overrides from wte_main
*/
void game_loop::end_game(void) {
    //
}
