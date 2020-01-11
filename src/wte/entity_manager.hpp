/*
  WTEngine
  By:  Matthew Evans
  File:  entity_manager.hpp

  See LICENSE.txt for copyright information

  Entity Manager class
*/

#ifndef WTE_ECS_ENTITY_MANAGER_HPP
#define WTE_ECS_ENTITY_MANAGER_HPP

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <limits>
#include <stdexcept>

#include "components\component.hpp"

namespace wte
{

namespace ecs
{

/*
  Define world/entity/component containers and iterators
*/
//!  Define entity type
typedef unsigned int entity;

//!  Define container for multiple entity reference
typedef std::vector<entity> world_container;
//!  Iterator for world storage
typedef std::vector<entity>::iterator world_iterator;

//!  Define container for components related to an entity
typedef std::vector<cmp::component_sptr> entity_container;
//!  Iterator for entity storage
typedef std::vector<cmp::component_sptr>::iterator entity_iterator;

//!  Define container for componets of same type
typedef std::map<entity, cmp::component_sptr> component_container;
//!  Iterator for component storage
typedef std::map<entity, cmp::component_sptr>::iterator component_iterator;

//!  Define main container for entity/component reference storage
typedef std::unordered_multimap<entity, cmp::component_sptr> world_map;

//! entity_manager class
/*!
  Store a collection of entities and their corresponding components in memory
*/
class entity_manager {
    public:
        entity_manager();                           /*!< Entity manager constructor */
        ~entity_manager();

        entity_manager(const entity_manager&) = delete;
        void operator=(entity_manager const&) = delete;

        void clear(void);

        //  Entity members
        const entity new_entity(void);                          /*!< Create a new entity */
        const bool delete_entity(const entity);                 /*!< Delete an entity */
        const bool entity_exists(const entity) const;           /*!< Check if an entity exists */
        const world_container get_entities(void) const;         /*!< Get all entities */
        const entity_container get_entity(const entity) const;  /*!< Get all components for an entity */

        //  Component members
        const bool add_component(const entity, const cmp::component_sptr);                  /*!< Add a component to an entity */
        template <typename T> const bool delete_component(const entity);
        template <typename T> const bool has_component(const entity) const;

        template <typename T> const std::shared_ptr<const T> get_component(const entity) const;   /*!< Get a single component for an entity */
        template <typename T> const std::shared_ptr<T> set_component(const entity);   /*!< Set a single component for an entity */

        template <typename T> const component_container get_components(void) const;         /*!< Get all components for a particulair type */

    private:
        entity entity_counter;          /*!< Store counter for entity creation */
        world_container entity_vec;     /*!< Store the list of entities */
        world_map world;                /*!< Store all components for every entity */
};

//! Entity Manager constructor
/*!
  Set the entity counter to zero and clear the entities and componenets
*/
inline entity_manager::entity_manager() {
    entity_counter = 0;
    entity_vec.clear();
    world.clear();
}

//! Entity Manager destructor
/*!
  Set the entity counter to zero and clear the entities and componenets
*/
inline entity_manager::~entity_manager() {
    entity_counter = 0;
    entity_vec.clear();
    world.clear();
}

//!  Clear the entity manager
/*!
  Set the entity counter to zero and clear the entities and componenets
*/
inline void entity_manager::clear(void) {
    entity_counter = 0;
    entity_vec.clear();
    world.clear();
}

//! Create new entity
/*!
  Create a new entity by name, using the next available ID
  Throw error if there is no room for entities.
  Return entity ID on success.
*/
inline const entity entity_manager::new_entity(void) {
    entity next_id;

    //  If the counter hits max, look for available ID
    if(entity_counter == std::numeric_limits<entity>::max()) {
        next_id = 0;

        for(world_iterator it = entity_vec.begin(); it != entity_vec.end(); it++) {
            //  No more room for entities, fail
            if(next_id == std::numeric_limits<entity>::max()) throw std::runtime_error("No room for more entities!");
            if(*it == next_id) {          //  Entity ID exists
                next_id++;                //  Increment next_id by one
                it = entity_vec.begin();  //  Then start the search over
            }
        }
    } else {  //  Counter not max, use the counter for entity ID
        next_id = entity_counter;
        entity_counter++;
    }

    entity_vec.push_back(next_id);
    return next_id; //  Created entity, return new entity ID
}

//! Delete entity by ID
/*!
  Return true on success, false if entity does not exist
*/
inline const bool entity_manager::delete_entity(const entity e_id) {
    for(world_iterator it = entity_vec.begin(); it != entity_vec.end(); it++) {
        if(*it == e_id) {
            world.erase(e_id); //  Remove all associated componenets
            entity_vec.erase(it); //  Delete the entity
            return true;
        }
    }
    return false;
}

//! Check if an entity exists
/*!
  Check the entity vector by ID and return result
  Return true if found
  Return false if not found
*/
inline const bool entity_manager::entity_exists(const entity e_id) const {
    if(entity_vec.empty()) return false;

    for(auto it = entity_vec.begin(); it != entity_vec.end(); it++) {
        if(*it == e_id) return true;
    }

    return false;
}

//! Get the entity reference vector
/*!
  Returns a vector of all entity IDs
*/
inline const world_container entity_manager::get_entities(void) const {
    if(entity_vec.empty()) return {};
    return entity_vec;
}

//! Get all components related to an entity
/*!
  Returns a container of components based by entity ID
*/
inline const entity_container entity_manager::get_entity(const entity e_id) const {
    if(!entity_exists(e_id)) return {};

    entity_container temp_container;
    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_sptr((*it).second));
    }

    return temp_container;
}

//! Add a new component to an entity
/*!
  Return false if the entity does not exist
  Return true on success
*/
inline const bool entity_manager::add_component(const entity e_id, const cmp::component_sptr comp) {
    if(!entity_exists(e_id)) return false;
    world.insert(std::make_pair(e_id, comp));
    return true;
}

//!  Delete all components by type for an entity
/*!
  Return true if a component was deleted
  Return false if no components were deleted
*/
template <typename T> inline const bool entity_manager::delete_component(const entity e_id) {
    if(world.empty()) throw std::runtime_error("No components were created!");

    bool deleted_component = false;
    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(dynamic_cast<T*>(it->second.get()) != nullptr) {
            it = world.erase(it);
            deleted_component = true;
        }
    }

    return deleted_component;
}

//!  Check if an entity has a component by type
/*!
  Return true if the entity has the component
  Return false if it does not
*/
template <typename T> inline const bool entity_manager::has_component(const entity e_id) const {
    if(world.empty()) throw std::runtime_error("No components were created!");

    bool has_component = false;
    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(dynamic_cast<T*>(it->second.get()) != nullptr) has_component = true;
    }

    return has_component;
}

//! Read the value of a component by type for an entity
/*!
  Return nullptr if not found
*/
template <typename T> inline const std::shared_ptr<const T> entity_manager::get_component(const entity e_id) const {
    if(world.empty()) throw std::runtime_error("No components were created!");

    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(dynamic_cast<T*>(it->second.get()) != nullptr) return std::dynamic_pointer_cast<const T>(it->second);
    }
    return nullptr;
}

//! Set the value of a component by type for an entity
/*!
  Return nullptr if not found
*/
template <typename T> inline const std::shared_ptr<T> entity_manager::set_component(const entity e_id) {
    if(world.empty()) throw std::runtime_error("No components were created!");

    auto results = world.equal_range(e_id);

    for(auto it = results.first; it != results.second; it++) {
        if(dynamic_cast<T*>(it->second.get()) != nullptr) return std::dynamic_pointer_cast<T>(it->second);
    }
    return nullptr;
}

//! Get all components for a particulair type
/*!
  Returns a container of components of all the same type
*/
template <typename T> inline const component_container entity_manager::get_components(void) const {
    if(world.empty()) throw std::runtime_error("No components were created!");
    
    component_container temp_components;

    for(auto it = world.begin(); it != world.end(); it++) {
        if(dynamic_cast<T*>(it->second.get()) != nullptr) {
            temp_components.insert(*it);
        }
    }

    return temp_components;
}

} //  namespace ecs

} //  namespace wte

#endif
