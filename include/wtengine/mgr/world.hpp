/*
 * wtengine
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_WORLD_HPP)
#define WTE_MGR_WORLD_HPP

#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <iterator>
#include <algorithm>
#include <limits>
#include <memory>
#include <mutex>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_debug/exceptions.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/cmp/component.hpp"

namespace wte {
  class engine;

  /* Define containers for entity/component/world storage. */
  /*!
  * \typedef std::size_t entity_id
  * Container to store an entity id.
  */
  using entity_id = std::size_t;

  /*!
  * \typedef std::pair<entity_id, std::string> entity
  * Container to store an entity reference.
  */
  using entity = std::pair<entity_id, std::string>;

  /*!
  * \typedef std::vector<entity> entities
  * Container for storing a group of entity references.
  */
  using entities = std::vector<entity>;

  /*!
  * \typedef std::vector<cmp::component_sptr> entity_container
  * Container to store a group of components related to an entity.
  */
  using entity_container = std::vector<cmp::component_sptr>;

  /*!
  * \typedef std::vector<cmp::component_csptr> const_entity_container
  * Constant container to store a group of components related to an entity.
  */
  using const_entity_container = std::vector<cmp::component_csptr>;

  /*!
  * Container for storing components of similar type.
  * \tparam Component type
  */
  template <typename T>
  using component_container = std::map<const entity_id, std::shared_ptr<T>>;

  /*!
  * Constant container for storing components of similar type.
  * \tparam Component type
  */
  template <typename T>
  using const_component_container = std::map<const entity_id, std::shared_ptr<const T>>;

  /*!
  * \typedef std::unordered_multimap<entity_id, cmp::component_sptr> world_map
  * Container to store the entire game world.
  */
  using world_map = std::unordered_multimap<entity_id, cmp::component_sptr>;
}

namespace wte::mgr {

/*!
 * \class world
 * \brief Store a collection of entities and their corresponding components in memory.
 */
class world final : private manager<world> {
  friend class wte::engine;

  public:
    /*!
     * \brief Create a new entity by name, using the next available ID.
     * \return The newly created entity ID.  WTE_ENTITY_ERROR on fail.
     */
    static entity_id new_entity(void) {
      entity_id next_id;

      if(entity_counter == ENTITY_MAX) {  //  Counter hit max.
        bool test = false;
        //  Look for the first available ID.
        for(next_id = ENTITY_START; !test; next_id++) {
          if(next_id == ENTITY_MAX) return ENTITY_ERROR;  //  No available ID, error.
          //  See if the new ID does not exist.
          test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&next_id](const entity& e){ return e.first == next_id; })
                  == entity_vec.end());
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
        test = (std::find_if(entity_vec.begin(), entity_vec.end(),
                            [&entity_name](const entity& e){ return e.second == entity_name; })
                == entity_vec.end());
        //  If it does, append the temp number and try that.
        if(!test) entity_name = "Entity" + std::to_string(next_id) + std::to_string(temp_id);
      }

      //  Tests complete, insert new entity.
      entity_vec.push_back(std::make_pair(next_id, entity_name));
      return next_id;  //  Return new entity ID.
    };

    /*!
     * \brief Delete entity by ID.
     * \param e_id The entity ID to delete.
     * \return Return true on success, false if entity does not exist.
     */
    static bool delete_entity(const entity_id& e_id) {
      auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&e_id](const entity& e){ return e.first == e_id; });
      if(e_it == entity_vec.end()) return false;

      _world.erase(e_id);      //  Remove all associated componenets.
      entity_vec.erase(e_it);  //  Delete the entity.

      return true;
    };

    /*!
     * \brief Check if an entity exists by ID.
     * \param e_id The entity ID to check.
     * \return Return true if found, return false if not found.
     */
    static bool entity_exists(const entity_id& e_id) {
      const bool result = (std::find_if(entity_vec.begin(), entity_vec.end(),
        [&e_id](const entity& e){ return e.first == e_id; })
        != entity_vec.end());
      return result;
    };

    /*!
     * \brief Get entity name.
     * \param e_id Entity ID to get name for.
     * \return Entity name string.
     * \exception wte_exception Entity does not exist.
     */
    static const std::string get_name(const entity_id& e_id) {
      auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&e_id](const entity& e){ return e.first == e_id; });
      if(e_it == entity_vec.end()) {
        //  Not found, throw error.
        throw engine_exception(
          "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
      }
      return e_it->second;
    };

    /*!
     * \brief Set the entity name.
     * \param e_id Entity ID to set.
     * \param name Entity name to set.
     * \return True if set, false on error.
     */
    static bool set_name(
      const entity_id& e_id,
      const std::string& name
    ) {
      auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&name](const entity& e){ return e.second == name; });
      if(n_it != entity_vec.end()) return false;  //  Entity with the new name exists, error.

      auto e_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&e_id](const entity& e){ return e.first == e_id; });
      if(e_it == entity_vec.end()) return false;  //  Didn't find entity_id, error.

      e_it->second = name;
      return true;
    };

    /*!
     * \brief Get entity ID by name.
     * \param name Name to search.
     * \return Entity ID, WTE_ENTITY_ERROR if not found.
     */
    static entity_id get_id(const std::string& name) {
      auto n_it = std::find_if(entity_vec.begin(), entity_vec.end(),
                              [&name](const entity& e){ return e.second == name; });
      if(n_it == entity_vec.end()) return ENTITY_ERROR;
      return n_it->first;
    };

    /*!
     * \brief Get the entity reference vector.
     * \return Returns a vector of all entity IDs and names.
     */
    static const entities get_entities(void) {
      entities temp_vec = entity_vec;
      return temp_vec;
    };

    /*!
     * \brief Set all components related to an entity.
     * \param e_id Entity ID to set components for.
     * \return Returns a container of components based by entity ID.
     * \exception wte_exception Entity does not exist.
     */
    static const entity_container set_entity(const entity_id& e_id) {
      if(!entity_exists(e_id)) {
        throw engine_exception(
          "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
      }

      entity_container temp_container;
      const auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_sptr((*it).second));
      }
      return temp_container;
    };

    /*!
     * \brief Get all components related to an entity.
     * \param e_id Entity ID to get components for.
     * \return Returns a constant container of components based by entity ID.
     * \exception wte_exception Entity does not exist.
     */
    static const const_entity_container get_entity(const entity_id& e_id) {
      if(!entity_exists(e_id)) {
        throw engine_exception(
          "Entity " + std::to_string(e_id) + " does not exist", "World", 4);
      }

      const_entity_container temp_container;
      const auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        temp_container.emplace_back(cmp::component_csptr((*it).second));
      }
      return temp_container;
    };

    /*!
     * \brief Add a component to an entity.
     * \tparam T Component type to add.
     * \param e_id Entity ID to add a component to.
     * \param args List of parameters to pass to component constructor.
     * \return Return false if the entity does not exist.
     * \return Return false if the entity already has a component of the same type.
     * \return Return true on success.
     */
    template <typename T, typename... Args>
    inline static bool add_component(
      const entity_id& e_id,
      Args... args
    ) {
      if(!entity_exists(e_id)) return false;

      //  Check derived types of existing components, make sure one does not already exist.
      const auto check_entity = get_entity(e_id);

      for(auto& it: check_entity) {
        auto& r = *it.get();
        if(typeid(r).name() == typeid(T).name()) return false;
      }

      _world.insert(std::make_pair(e_id, std::make_shared<T>(args...)));
      return true;
    };

    /*!
     * \brief Delete a component by type for an entity.
     * \tparam T Component type to delete.
     * \param e_id Entity ID to delete component from.
     * \return Return true if a component was deleted.
     * \return Return false if no components were deleted.
     */
    template <typename T>
    inline static bool delete_component(const entity_id& e_id) {
      auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second)) {
          it = _world.erase(it);
          return true;
        }
      }
      return false;
    };

    /*!
     * \brief Check if an entity has a component by type.
     * \tparam T Component type to check.
     * \param e_id The entity ID to check.
     * \return Return true if the entity has the component.
     * \return Return false if it does not.
     */
    template <typename T>
    inline static bool has_component(const entity_id& e_id) {
      const auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second))
          return true;
      }
      return false;
    };

    /*!
     * \brief Set the value of a component by type for an entity.
     * \tparam T Component type to search.
     * \param e_id The entity ID to search.
     * \return Return the component.
     * \exception wte_exception Component not found.
     */
    template <typename T>
    inline static const std::shared_ptr<T> set_component(const entity_id& e_id) {
      const auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second))
          return std::static_pointer_cast<T>(it->second);
      }

      throw engine_exception(
        "Entity: " + std::to_string(e_id) + " - Component not found", "World", 4);
    };

    /*!
     * \brief Read the value of a component by type for an entity.
     * \tparam T Component type to search.
     * \param e_id The entity ID to search.
     * \return Return the component.
     * \exception wte_exception Component not found.
     */
    template <typename T>
    inline static const std::shared_ptr<const T> get_component(const entity_id& e_id) {
      const auto results = _world.equal_range(e_id);

      for(auto it = results.first; it != results.second; it++) {
        if(std::dynamic_pointer_cast<T>(it->second))
          return std::static_pointer_cast<const T>(it->second);
      }

      throw engine_exception(
        "Entity: " + std::to_string(e_id) + " - Component not found", "World", 4);
    };

    /*!
     * \brief Return a 'set' container for all components for a particulair type.
     * \tparam T Component type to search.
     * \return Returns a container of components of all the same type.
     */
    template <typename T>
    inline static const component_container<T> set_components(void) {
      component_container<T> temp_components;

      for(auto& it: _world) {
        if(std::dynamic_pointer_cast<T>(it.second))
          temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
      }
      return temp_components;
    };

    /*!
     * \brief Return a 'get' container for all components for a particulair type.
     * \tparam T Component type to search.
     * \return Returns a constant container of components of all the same type.
     */
    template <typename T>
    inline static const const_component_container<T> get_components(void) {
      const_component_container<T> temp_components;

      for(auto& it: _world) {
        if(std::dynamic_pointer_cast<T>(it.second))
          temp_components.insert(std::make_pair(it.first, std::static_pointer_cast<T>(it.second)));
      }
      return temp_components;
    };

    inline static const entity_id ENTITY_ERROR = 0;  //!<  Entity error code.
    inline static const entity_id ENTITY_START = 1;  //!<  Start of Entity counter.
    inline static const entity_id ENTITY_MAX =       //!<  Entity max value.
      std::numeric_limits<entity_id>::max();

  private:
    world() = default;
    ~world() = default;

    //  Clear the entity manager.
    static void clear(void) {
      entity_counter = ENTITY_START;
      entity_vec.clear();     //  Clear entities vector
      _world.clear();         //  Clear the world block
    };

    static entity_id entity_counter = ENTITY_START;  //  Last Entity ID used.
    static entities entity_vec;                      //  Container for all entities.
    static world_map _world;                         //  Container for all components.
};

//template <> bool manager<world>::initialized = false;

}  //  namespace wte::mgr

#endif
