/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_input.cpp

  See LICENSE.txt for copyright information

  Custom spawner code
  This overrides the spawner system to define custom behavior
*/

#define _USE_MATH_DEFINES

#include <cmath>

#include "include/custom_spawner.hpp"

using namespace wte;

/*
  Process the spawn messages
*/
void custom_spawner::process_spawn(mgr::entity_manager& world, msg_arg_list args) {
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
void custom_spawner::new_asteroid(mgr::entity_manager& world, float x, float y, float d, float v, int s) {
    entity e_id;

    if(s < 1) s = 1;
    if(s > 8) s = 8;

    e_id = world.new_entity();
    world.add_component(e_id, std::make_shared<cmp::name>("asteroid" + std::to_string(e_id)));
    world.add_component(e_id, std::make_shared<cmp::team>(1));
    world.add_component(e_id, std::make_shared<cmp::location>(x, y));
    world.add_component(e_id, std::make_shared<cmp::hitbox>((float)(s * 16), (float)(s * 16)));
    world.add_component(e_id, std::make_shared<cmp::health>(s * 10));
    world.add_component(e_id, std::make_shared<cmp::damage>(10));
    world.add_component(e_id, std::make_shared<cmp::direction>(d));
    world.add_component(e_id, std::make_shared<cmp::velocity>(v));
    world.add_component(e_id, std::make_shared<cmp::render_order>(0));
    world.add_component(e_id, std::make_shared<cmp::visible>());
    world.add_component(e_id, std::make_shared<cmp::enabled>());
    world.add_component(e_id, std::make_shared<cmp::ai>(
        [](entity ast_id, mgr::entity_manager& world, mgr::message_manager& messages, int64_t engine_time) {
            //  AI for asteroids.  Move them at their speed and angle.
            world.set_component<cmp::location>(ast_id)->pos_x +=
                world.get_component<cmp::velocity>(ast_id)->speed *
                cos(world.get_component<cmp::direction>(ast_id)->angle * (M_PI / 180));

            world.set_component<cmp::location>(ast_id)->pos_y +=
                world.get_component<cmp::velocity>(ast_id)->speed *
                sin(world.get_component<cmp::direction>(ast_id)->angle * (M_PI / 180));

            //  OOB check.
            if(world.get_component<cmp::location>(ast_id)->pos_y > engine_cfg::get<int>("screen_height") + 100) {
                messages.add_message(message("spawner", "delete",
                                     world.get_component<cmp::name>(ast_id)->name_str));
            }

            //  Health check.  Asteroid's HP is 0, reward player with points and delete the entity.
            if(world.get_component<cmp::health>(ast_id)->hp == 0) {
                messages.add_message(message("spawner", "delete", world.get_component<cmp::name>(ast_id)->name_str));
                game_cfg_map::add<int>("score", 10);
            }
        }
    ));
    world.add_component(e_id, std::make_shared<cmp::dispatcher>(
        [](entity ast_id, mgr::entity_manager& world, message msg, int64_t engine_time) {
            //  Process colision messages
            if(msg.get_cmd() == "colision") {
                //  Main cannon hit the enemy.
                if(msg.get_from() == "main_cannon") world.set_component<cmp::health>(ast_id)->hp -= 1;
                //  Shield hit the enemy.
                if(msg.get_from() == "shield") world.set_component<cmp::health>(ast_id)->hp -= 10;
            } //  end colision messages
        }
    ));
}
