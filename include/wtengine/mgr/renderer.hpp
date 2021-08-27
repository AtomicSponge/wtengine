/*!
 * WTEngine | File:  renderer.hpp
 * 
 * \author Matthew Evans
 * \version 0.6
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#ifndef WTE_MGR_RENDERER_HPP
#define WTE_MGR_RENDERER_HPP

#include <string>
#include <utility>
#include <set>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <cassert>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wtengine/mgr/manager.hpp"

#include "wtengine/_globals/_defines.hpp"
#include "wtengine/_globals/alert.hpp"
#include "wtengine/_globals/engine_time.hpp"
#include "wtengine/_globals/wrappers.hpp"
#include "wtengine/_globals/wte_asset.hpp"
#include "wtengine/_globals/wte_exception.hpp"
#include "wtengine/cmp/_components.hpp"
#include "wtengine/mgr/menus.hpp"
#include "wtengine/mgr/world.hpp"
#include "wtengine/config.hpp"
#include "wtengine/display.hpp"

namespace wte {

class display;
class engine;

namespace mgr {

/*!
 * Container for an entity and component pair.  Used for sorting.
 * \tparam Component type
 */
template <typename T>
using entity_component_pair = std::pair<const entity_id, std::shared_ptr<const T>>;

/*!
 * \class renderer
 * \brief An object that handles drawing the world to the screen.
 */
class renderer final : private manager<renderer> {
    friend class wte::display;
    friend class wte::engine;

    public:
        /*!
         * \brief Set the arena size.
         * 
         * Once the renderer has been initialized it can not be changed.
         * This should be called during engine initialization before the main object is created.
         * 
         * \param w Arena width in pixels.
         * \param h Arena height in pixels.
         */
        static void set_arena_size(
            const int& w,
            const int& h
        );

        /*!
         * \brief Get the arnea width.
         * \return The arena width in pixels.
         */
        static const int get_arena_width(void);

        /*!
         * \brief Get the arena height.
         * \return The arena height in pixels.
         */
        static const int get_arena_height(void);

        /*!
         * \brief Set the title screen.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param fname Filename of the title screen.
         */
        static void set_title_screen(
            const std::string& fname
        );

        /*!
         * \brief Set the font to be used by the renderer.
         * 
         * If not called, Allegro's default font will be used.
         * This should be called during engine initialization before the main object is created.
         * See the Allegro docs for more information on al_load_font and the parameters used.
         * 
         * \param fone Font asset to use.
         */
        static void set_font(
            wte_asset<al_font> font
        );

    private:
        renderer() = default;
        ~renderer() = default;

        /* 
         * Configures the internal objects for the render manager to use.
         */
        static void initialize(void);

        /* 
         * Destories the internal objects.
         */
        static void de_init(void);

        /*
         * Inform the renderer of the screen resolution.
         * Gets called by the engine when the screen resolution is updated.
         */
        static void set_resolution(
            const int& w,
            const int& h
        );

        /*
         * Draw the game screen.
         */
        static void render(void);

        template <typename T> struct comparator {
            const bool operator() (const T& a, const T& b) const {
                return *a.second < *b.second;
            }
        };

        static ALLEGRO_TIMER* fps_timer;
        static ALLEGRO_EVENT_QUEUE* fps_event_queue;
        static ALLEGRO_EVENT fps_event;

        static wte_asset<al_bitmap> arena_bitmap;
        static wte_asset<al_bitmap> title_bitmap;
        static wte_asset<al_font> renderer_font;

        static std::size_t fps_counter, fps;

        static int screen_w, screen_h;
        static int arena_w, arena_h;
        static bool arena_created;

        static std::string title_screen_file;
        static std::string background_file;
};

} //  namespace mgr

} //  namespace wte

#endif
