/*
 * silvergun
 * --------
 * By Matthew Evans
 * See LICENSE.md for copyright information.
 */

#if !defined(WTE_MGR_ASSETS_HPP)
#define WTE_MGR_ASSETS_HPP

#include <string>
#include <tuple>
#include <map>
#include <exception>

#include <allegro5/allegro.h>

#include "silvergun/mgr/manager.hpp"

#include "silvergun/_debug/exceptions.hpp"
#include "silvergun/_globals/_defines.hpp"
#include "silvergun/_globals/engine_time.hpp"
#include "silvergun/_globals/wte_asset.hpp"

namespace wte {
  class engine;
}

namespace wte::mgr {

template <typename T>
using asset_map = std::map<const std::string, wte_asset<T>>;

/*!
 * \class assets
 * \brief Stores an index of assets.
 * \tparam ...Types Types of assets used in game code.
 */
class assets final : private manager<assets> {
  friend class wte::engine;

  private:
    assets() = default;
    ~assets() = default;

    //  Engine calls this during de-init to clean up.
    inline static void clear_al_objects(void) {
      for (auto& it : _assets<ALLEGRO_BITMAP>) {
        it.second.reset();
      }
      for (auto& it : _assets<ALLEGRO_FONT>) {
        it.second.reset();
      }
      for (auto& it : _assets<ALLEGRO_SAMPLE>) {
        it.second.reset();
      }
      for (auto& it : _assets<ALLEGRO_AUDIO_STREAM>) {
        it.second.reset();
      }

      _assets<ALLEGRO_BITMAP>.clear();
      _assets<ALLEGRO_FONT>.clear();
      _assets<ALLEGRO_SAMPLE>.clear();
      _assets<ALLEGRO_AUDIO_STREAM>.clear();
    };

    //  Store each asset map.
    template <typename T>
    inline static asset_map<T> _assets;

  public:
    /*!
     * \brief Load an existing asset.
     * \tparam T Asset type to add.
     * \param label Reference label for asset.
     * \param obj Asset to add.
     * \return True if loaded.  False if not.
     */
    template <typename T>
    inline static bool load(
      const std::string& label,
      const wte_asset<T> obj
    ) {
      auto ret = _assets<T>.insert(std::make_pair(label, std::move(obj)));
      return ret.second;
    };

    /*!
     * \brief Unload an asset.
     * \tparam T Asset type to unload.
     * \param label Reference label for asset.
     * \return True if removed, false if not.
     */
    template <typename T>
    inline static bool unload(
      const std::string& label
    ) {
      auto it = _assets<T>.find(label);
      if (it != _assets<T>.end()) {
        _assets<T>.erase(it);
        return true;
      }
      return false;
    };

    /*!
     * \brief Get an asset by reference label.
     * \tparam T Asset type to get.
     * \param label Reference label for asset.
     * \return Shared pointer to asset.
     * \exception Asset not found.
     */
    template <typename T>
    inline static const wte_asset<T> get(
      const std::string& label
    ) {
      try {
        return _assets<T>.at(label);
      } catch(std::out_of_range& e) {
        std::string err_msg = "Asset not found: " + label;
        throw engine_exception(err_msg, "Assets", 4);
      } catch(...) {
        std::string err_msg = "Asset error: " + label;
        throw engine_exception(err_msg, "Assets", 4);
      }
    };
};

template <> inline bool manager<assets>::initialized = false;

}  //  end namespace wte::mgr

#endif
