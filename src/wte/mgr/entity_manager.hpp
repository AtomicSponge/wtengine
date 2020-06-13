/*!
 * \brief WTEngine | File:  entity_manager.hpp
 * \author Matthew Evans
 *
 * \version 0.1a
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2020
 *
 * \details Entity Manager class.
 */

#ifndef WTE_MGR_ENTITY_MANAGER_HPP
#define WTE_MGR_ENTITY_MANAGER_HPP

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <iterator>
#include <limits>
#include <stdexcept>

#include "manager.hpp"
#include "../cmp/component.hpp"

namespace wte
{

/*
 * Define world/entity/component containers and iterators.
 */
//!  Container to store an entity reference.
typedef unsigned int entity;

//!  Container for storing a group of entitie references.
typedef std::vector<entity> world_container;  //  TODO: clean up
//!  Iterator for addressing a group of entities.
typedef std::vector<entity>::iterator world_iterator;
//!  Constant iterator for addressing a group of entities.
typedef std::vector<entity>::const_iterator world_citerator;

//!  Container to store a group of components related to an entity.
typedef std::vector<cmp::component_sptr> entity_container;  //  TODO: clean these up too
//!  Iterator for addressing a group of components related to an entity.
typedef std::vector<cmp::component_sptr>::iterator entity_iterator;
//!  Constant iterator for addressing a group of components related to an entity.
typedef std::vector<cmp::component_sptr>::const_iterator entity_citerator;

//!  Container for storing components of similar type.
//typedef std::map<entity, cmp::component_sptr> component_container;
template <typename T> using component_container = std::map<const entity, std::shared_ptr<T>>;
//!  Constant container for storing components of similar type.
template <typename T> using const_component_container = std::map<const entity, std::shared_ptr<const T>>;
//!  Iterator for component storage.
//typedef std::map<entity, cmp::component_sptr>::iterator component_iterator;

//!  Container to store all entites.
typedef std::unordered_multimap<entity, cmp::component_sptr> world_map;

namespace mgr
{

/*!
 * \brief Entity manager class.
 * Store a collection of entities and their corresponding components in memory.
 */
class entity_manager final : public manager<entity_manager> {
    public:
        inline entity_manager() { clear(); };
        inline ~entity_manager() { clear(); };

        /*!
         * \brief Clear the entity manager.
         * Set the entity counter to zero and clear the entities and componenets.
         * \param void
         * \return void
         */
        inline void clear(void) {
            entity_counter = 0;
            entity_vec.clear();
            world.clear();
        };

        /*!
         * \brief Create a new entity by name, using the next available ID.
         * Throw error if there is no room for entities.
         * \param void
         * \return Return the newly created entity ID.
         */
        inline const entity new_entity(void) {
            entity next_id;

            //  If the counter hits max, look for available ID
            if(entity_counter == std::numeric_limits<entity>::max()) {
                next_id = 0;

                for(world_iterator it = entity_vec.begin(); it != entity_vec.end(); it++) {
                    //  No more room for entities, fail.
                    if(next_id == std::numeric_limits<entity>::max())
                        throw std::runtime_error("Entity manager has hit max!");
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
        };

        /*!
         * Delete entity by ID.
         * \param e_id The entity ID to delete.
         * \return Return true on success, false if entity does not exist.
         */
        inline const bool delete_entity(const entity e_id) {
            for(world_iterator it = entity_vec.begin(); it != entity_vec.end(); it++) {
                if(*it == e_id) {
                    world.erase(e_id); //  Remove all associated componenets
                    entity_vec.erase(it); //  Delete the entity
                    return true;
                }
            }
            return false;
        };

        /*!
         * \brief Check if an entity exists.
         * Check the entity vector by ID and return result.
         * \param e_id The entity ID to check.
         * \return Return true if found, return false if not found.
         */
        inline const bool entity_exists(const entity e_id) const {
            if(entity_vec.empty()) return false;

            for(auto it = entity_vec.begin(); it != entity_vec.end(); it++) {
                if(*it == e_id) return true;
            }

            return false;
        };

        /*!
         * Get the entity reference vector.
         * \param void
         * \return Returns a vector of all entity IDs.
         */
        inline const world_container get_entities(void) const {
            if(entity_vec.empty()) return {};
            return entity_vec;
        };

        /*!
         * Get all components related to an entity.
         * \param e_id Entity ID to get components for.
         * \return Returns a container of components based by entity ID.
         */
        inline const entity_container get_entity(const entity e_id) const {
            if(!entity_exists(e_id)) return {};

            entity_container temp_container;
            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                temp_container.emplace_back(cmp::component_sptr((*it).second));
            }

            return temp_container;
        };

        /*!
         * Add a new component to an entity
         * \param e_id Entity ID to add a component to.
         * \param comp Componenet to add.
         * \return Return false if the entity does not exist.
         * \return Return false if the entity already has a component of the same type.
         * \return Return true on success.
         */
        inline const bool add_component(const entity e_id, const cmp::component_sptr comp) {
            if(!entity_exists(e_id)) return false;

            //  Check derived types of existing components, make sure one does not already exist
            entity_container check_entity = get_entity(e_id);
            for(entity_iterator it = check_entity.begin(); it != check_entity.end(); it++) {
                if(typeid(**it).name() == typeid(*comp).name()) return false;
            }

            world.insert(std::make_pair(e_id, comp));
            return true;
        };

        /*!
         * Delete components by type for an entity.
         * \param e_id Entity ID to delete component from.
         * \return Return true if a component was deleted.
         * \return Return false if no components were deleted.
         */
        template <typename T> inline const bool delete_component(const entity e_id) {
            if(world.empty()) throw std::runtime_error("No components were created!");

            bool deleted_component = false;
            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second;) {
                if(std::dynamic_pointer_cast<T>(it->second)) {
                    it = world.erase(it);
                    deleted_component = true;
                } else it++;
            }

            return deleted_component;
        };

        /*!
         * Check if an entity has a component by type.
         * \param e_id The entity ID to check.
         * \return Return true if the entity has the component.
         * \return Return false if it does not.
         */
        template <typename T> inline const bool has_component(const entity e_id) const {
            if(world.empty()) throw std::runtime_error("No components were created!");

            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second)) return true;
            }

            return false;
        };

        /*!
         * Read the value of a component by type for an entity.
         * \param e_id The entity ID to search.
         * \return Return the component or nullptr if not found.
         */
        template <typename T> inline const std::shared_ptr<const T> get_component(const entity e_id) const {
            if(world.empty()) throw std::runtime_error("No components were created!");

            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<const T>(it->second);
            }
            return nullptr;
        };

        /*!
         * Set the value of a component by type for an entity.
         * \param e_id The entity ID to search.
         * \return Return the component or nullptr if not found.
         */
        template <typename T> inline const std::shared_ptr<T> set_component(const entity e_id) {
            if(world.empty()) throw std::runtime_error("No components were created!");

            auto results = world.equal_range(e_id);

            for(auto it = results.first; it != results.second; it++) {
                if(std::dynamic_pointer_cast<T>(it->second))
                    return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        };

        /*!
         * Set all components for a particulair type.
         * \param void
         * \return Returns a container of components of all the same type.
         */
        template <typename T> inline const component_container<T> set_components(void) {
            if(world.empty()) throw std::runtime_error("No components were created!");

            component_container<T> temp_components;

            for(auto & it : world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }

            return temp_components;
        };

        /*!
         * Get all components for a particulair type.
         * \param void
         * \return Returns a constant container of components of all the same type.
         */
        template <typename T> inline const const_component_container<T> get_components(void) const {
            if(world.empty()) throw std::runtime_error("No components were created!");

            const_component_container<T> temp_components;

            for(auto & it : world) {
                if(std::dynamic_pointer_cast<T>(it.second))
                    temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
            }

            return temp_components;
        };

    private:
        entity entity_counter;
        world_container entity_vec;
        world_map world;
};

template <> inline bool manager<entity_manager>::initialized = false;

} //  namespace mgr

} //  namespace wte

#endif
