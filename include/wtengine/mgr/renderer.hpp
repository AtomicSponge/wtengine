/*!
 * WTEngine | File:  renderer.hpp
 * 
 * \author Matthew Evans
 * \version 0.71
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
#include <chrono>
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
#include "wtengine/mgr/world.hpp"
#include "wtengine/config.hpp"
#include "wtengine/display.hpp"

namespace wte {

class display;
class engine;

template <typename T>
using time_point = std::chrono::time_point<T>;

typedef std::chrono::system_clock system_clock;
typedef std::chrono::system_clock::duration duration;

}

namespace wte::mgr::gfx {

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
         * \brief Set the title screen.
         * 
         * This should be called during engine initialization before the main object is created.
         * 
         * \param fname Filename of the title screen.
         */
        static void set_title_screen(const std::string& fname);

        /*!
         * \brief Set the font to be used by the renderer.
         * 
         * If not called, Allegro's default font will be used.
         * This should be called during engine initialization before the main object is created.
         * See the Allegro docs for more information on al_load_font and the parameters used.
         * 
         * \param fone Font asset to use.
         */
        static void set_font(wte_asset<al_font> font);

        static const std::size_t& fps;                       //!<  Frames per second
        static const time_point<system_clock>& last_render;  //!<  Point in time last render completed
        static const time_point<system_clock>& start_time;   //!<  Point in time the renderer started
        static const duration& delta_time;                   //!<  Time between frame renders

    private:
        renderer() = default;
        ~renderer() = default;

        //  Configures the internal objects for the render manager to use.
        static void initialize(void);
        //  Destories the internal objects.
        static void de_init(void);
        //  Draw the game screen.
        static void render(void);

        template <typename T> struct comparator {
            const bool operator() (const T& a, const T& b) const {
                return *a.second < *b.second;
            }
        };

        inline static void draw_hitboxes(void) {
            if constexpr (build_options.debug_mode) {
                const const_component_container<cmp::hitbox> hitbox_components =
                    mgr::world::get_components<cmp::hitbox>();

                for(auto& it: hitbox_components) {
                    if(it.second->solid) {
                        //  Select color based on team.
                        ALLEGRO_COLOR team_color;
                        switch(it.second->team) {
                            case 0: team_color = WTE_COLOR_GREEN; break;
                            case 1: team_color = WTE_COLOR_RED; break;
                            case 2: team_color = WTE_COLOR_BLUE; break;
                            default: team_color = WTE_COLOR_YELLOW;
                        }
                        //  Draw the hitbox.
                        ALLEGRO_BITMAP* temp_bitmap = al_create_bitmap(
                            it.second->width,
                            it.second->height);
                        al_set_target_bitmap(temp_bitmap);
                        al_clear_to_color(team_color);
                        al_set_target_bitmap(**arena_bitmap);
                        try {
                            al_draw_bitmap(temp_bitmap,
                                mgr::world::get_component<cmp::location>(it.first)->pos_x,
                                mgr::world::get_component<cmp::location>(it.first)->pos_y, 0);
                        } catch(const wte_exception& e) { alert::set(e.what(), e.where(), e.when()); }
                        al_destroy_bitmap(temp_bitmap);
                    }
                }
            }
        };

        static ALLEGRO_TIMER* fps_timer;
        static ALLEGRO_EVENT_QUEUE* fps_event_queue;
        static ALLEGRO_EVENT fps_event;

        static wte_asset<al_bitmap> arena_bitmap;
        static wte_asset<al_bitmap> title_bitmap;
        static wte_asset<al_font> renderer_font;

        static std::size_t fps_counter, _fps;
        static time_point<system_clock> _last_render, _start_time;
        static duration _delta_time;

        static bool arena_created;

        static std::string title_screen_file;
        static std::string background_file;
};

}  //  end namespace wte::mgr

#endif
