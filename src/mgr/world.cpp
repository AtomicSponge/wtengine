/*!
 * wtengine | File:  world.cpp
 * 
 * \author Matthew Evans
 * \version 0.7.2
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2022
 */

#include "wtengine/mgr/world.hpp"

namespace wte::mgr {

template <> bool manager<world>::initialized = false;

entity_id world::entity_counter = ENTITY_START;
entities world::entity_vec;
world_map world::_world;

std::mutex world::entity_mtx;
std::mutex world::world_mtx;

/*
 *
 */
void world::clear(void) {
    entity_counter = ENTITY_START;
    entity_mtx.lock();
    entity_vec.clear();
    entity_mtx.unlock();
    world_mtx.lock();
    _world.clear();
    world_mtx.unlock();
}

/*
 *
 */
const entity_id world::new_entity(void) {
    entity_id next_id;

    if(entity_counter == ENTITY_MAX) {  //  Counter hit max.
        bool test = false;
        //  Look for the first available ID.
        for(next_id = ENTITY_START; !test; next_id++) {
            if(next_id == ENTITY_MAX) return ENTITY_ERROR;  //  No available ID, error.
            //  See if the new ID does not exist.
            entity_mtx.lock();
            test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                                 [&next_id](const entity& e){ return e.first == next_id; })
                    == entity_vec.end());
            entity_mtx.unlock();
        }
    } else {  //  Counter not max, use the counter for entity ID.
        next_id = entity_counter;
        entity_counter++;
    }

    //  Set a new name.  Make sure name doesn't exist.
    std::string entity_name = "Entity" + std::to_string(next_id);
    bool test = false;
    for(entity_id temp_id = ENTITY_START; !test; temp_id++) {
        if(temp_id == ENTITY_MAX) return ENTITY_ERROR;  //  Couldn't name entity, error.
        //  See if the new name does not exist.
        entity_mtx.lock();
        test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&entity_name](const entity& e){ return e.second == entity_name; })
                == entity_vec.end());
        entity_mtx.unlock();
        //  If it does, append the temp number and try that.
        if(!test) entity_name = "Entity" + std::to_string(next_id) + std::to_string(temp_id);
    }

    //  Tests complete, insert new entity.
    entity_mtx.lock();
    entity_vec.push_back(std::make_pair(next_id, entity_name));
    entity_mtx.unlock();
    return next_id;  //  Return new entity ID.
}

/*
 *
 */
const bool world::delete_entity(const entity_id& e_id) {
    entity_mtx.lock();
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    if(e_it == entity_vec.end()) return false;
    entity_mtx.unlock();
    world_mtx.lock();
    _world.erase(e_id);      //  Remove all associated componenets.
    world_mtx.unlock();
    entity_mtx.lock();
    entity_vec.erase(e_it);  //  Delete the entity.
    entity_mtx.unlock();
    return true;
}

/*
 *
 */
const bool world::entity_exists(const entity_id& e_id) {
    entity_mtx.lock();
    const bool result = (std::find_if(entity_vec.begin(), entity_vec.end(),
        [&e_id](const entity& e){ return e.first == e_id; })
        != entity_vec.end());
    entity_mtx.unlock();
    return result;
}

/*
 *
 */
const std::string world::get_name(const entity_id& e_id) {
    entity_mtx.lock();
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    entity_mtx.unlock();
    if(e_it == entity_vec.end()) {
        //  Not found, throw error.
        throw exception(
            exception_item("Entity " + std::to_string(e_id) + " does not exist", "World", 4));
    }
    return e_it->second;
}

/*
 *
 */
const bool world::set_name(const entity_id& e_id, const std::string& name) {
    entity_mtx.lock();
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&name](const entity& e){ return e.second == name; });
    entity_mtx.unlock();
    if(n_it != entity_vec.end()) return false;  //  Entity with the new name exists, error.
    entity_mtx.lock();
    auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&e_id](const entity& e){ return e.first == e_id; });
    entity_mtx.unlock();
    if(e_it == entity_vec.end()) return false;  //  Didn't find entity_id, error.
    e_it->second = name;
    return true;
}

/*
 *
 */
const entity_id world::get_id(const std::string& name) {
    entity_mtx.lock();
    auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                             [&name](const entity& e){ return e.second == name; });
    entity_mtx.unlock();
    if(n_it == entity_vec.end()) return ENTITY_ERROR;
    return n_it->first;
}

/*
 *
 */
const entities world::get_entities(void) {
    entity_mtx.lock();
    entities temp_vec = entity_vec;
    entity_mtx.unlock();
    return temp_vec;
}

/*
 *
 */
const entity_container world::set_entity(const entity_id& e_id) {
    if(!entity_exists(e_id)) {
        throw exception(
            exception_item("Entity " + std::to_string(e_id) + " does not exist", "World", 4));
    }

    entity_container temp_container;
    world_mtx.lock();
    const auto results = _world.equal_range(e_id);
    world_mtx.unlock();

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_sptr((*it).second));
    }
    return temp_container;
}

/*
 *
 */
const const_entity_container world::get_entity(const entity_id& e_id) {
    if(!entity_exists(e_id)) {
        throw exception(
            exception_item("Entity " + std::to_string(e_id) + " does not exist", "World", 4));
    }

    const_entity_container temp_container;
    world_mtx.lock();
    const auto results = _world.equal_range(e_id);
    world_mtx.unlock();

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_csptr((*it).second));
    }
    return temp_container;
}

}  //  end namespace wte::mgr
