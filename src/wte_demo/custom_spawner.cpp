/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_input.cpp

  See LICENSE.txt for copyright information

  Custom spawner code
  This overrides the spawner system to define custom behavior
*/

#include "include/custom_spawner.hpp"

/*
  Process the spawn messages
*/
void custom_spawner::process_spawn(wte::ecs::entity_manager& world, wte::msg::arg_list args) {
    //  Spawn an asteroid
    if(args[0] == "asteroid") {
        if(args.size() == 6) new_asteroid(world, std::stof(args[1]), std::stof(args[2]),
                                          std::stof(args[3]), std::stof(args[4]), std::stoi(args[5]));
    }
}

/*
  Create an asteroid entity
  Passed:
   - entity manager, (f)pos_x, (f)pos_y, (f)direction, (f)velocity, (i)size
*/
void custom_spawner::new_asteroid(wte::ecs::entity_manager& world, float x, float y, float d, float v, int s) {
    wte::ecs::entity e_id;

    if(s < 1) s = 1;
    if(s > 8) s = 8;

    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::name>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::team>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::location>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::hitbox>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::health>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::damage>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::direction>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::velocity>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::render_order>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::visible>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::enabled>());
    world.add_component(e_id, std::make_shared<wte::ecs::cmp::ai>());
    world.set_component<wte::ecs::cmp::name>(e_id)->name = "asteroid" + std::to_string(e_id);
    world.set_component<wte::ecs::cmp::team>(e_id)->team = 1;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_x = x;
    world.set_component<wte::ecs::cmp::location>(e_id)->pos_y = y;
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->width = (float)(s * 16);
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->height = (float)(s * 16);
    world.set_component<wte::ecs::cmp::hitbox>(e_id)->solid = true;
    world.set_component<wte::ecs::cmp::health>(e_id)->hp = s * 10;
    world.set_component<wte::ecs::cmp::damage>(e_id)->dmg = 10;
    world.set_component<wte::ecs::cmp::direction>(e_id)->angle = d;
    world.set_component<wte::ecs::cmp::velocity>(e_id)->speed = v,
    world.set_component<wte::ecs::cmp::render_order>(e_id)->order = 0;
    world.set_component<wte::ecs::cmp::visible>(e_id)->is_visible = true;
    world.set_component<wte::ecs::cmp::enabled>(e_id)->is_enabled = true;
}

/*
  Create a wall entity
*/
void custom_spawner::new_wall(wte::ecs::entity_manager& world, float x, float y) {
    //
}
