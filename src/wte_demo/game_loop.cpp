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
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>("starfield"));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::background>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::background_layer>(0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());

    //  Player entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>("player"));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>(0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>((wte::screen_width / 2) - 5,
                                                                         wte::screen_height - 40));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>(10, 10));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::health>(1));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>(1));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>());

    //  Main cannon entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>("main_cannon"));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>(0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>(0, 0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>(10, 200, false));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::damage>(5));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>(2));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>(false));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>(false));

    //  Shield entity
    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>("shield"));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>(0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>(0, 0));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>(40, 40, false));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::damage>(100));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>(2));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::input_handler>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>(false));
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>(false));
}

/*
  Stop the game

  Overrides from wte_main
*/
void game_loop::end_game(void) {
    //
}
