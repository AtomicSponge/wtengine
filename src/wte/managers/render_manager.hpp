/*
  WTEngine
  By:  Matthew Evans
  File:  render_manager.hpp

  See LICENSE.txt for copyright information

  render_manager object
*/

#ifndef WTE_MGR_RENDER_MANAGER_HPP
#define WTE_MGR_RENDER_MANAGER_HPP

#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "manager.hpp"

#include "..\wte_global_defines.hpp"
#include "..\wte_config.hpp"
#include "..\engine_flags.hpp"
#include "..\components\components.hpp"
#include "..\alert.hpp"
#include "menu_manager.hpp"
#include "entity_manager.hpp"

namespace wte
{

namespace mgr
{

//!  Define a container for an entity and component pair
typedef std::pair<entity, cmp::component_sptr> entity_component_pair;
//!  Iterator for the entity/component pair
typedef std::set<entity_component_pair>::iterator ec_pair_iterator;
//!  Comparator for sorting entity/component pairs
typedef std::function<bool(entity_component_pair, entity_component_pair)> comparator;

//! render_manager class
/*!
  An object that handles drawing the world to the screen
*/
class render_manager final : public manager<render_manager> {
    public:
        //! render_manager constructor
        render_manager();
        //! render_manager destructor
        ~render_manager();

        //!  Initialize the render_manager
        void initialize(ALLEGRO_FONT*);
        //!  Render method - Draw the game screen
        void render(menu_manager&, entity_manager&);     /*!< Call the render_manager */

        #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
        //!  Tracks the engine timer if debugging is enabled
        inline void set_current_time(const int64_t ctime) { current_time = ctime; };
        #endif

    private:
        ALLEGRO_BITMAP* render_tmp_bmp;
        ALLEGRO_FONT* overlay_font;                     /*!< Allegro font used for the overlay */

        ALLEGRO_TIMER* fps_timer;
        ALLEGRO_EVENT_QUEUE* fps_event_queue;
        ALLEGRO_EVENT fps_event;

        int fps_counter, fps;                           /*!< FPS counters */
        
        comparator render_comparator;                   /*!< Store lambda function for comparator */

        #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
        int64_t current_time;
        #endif
};

template <> inline bool render_manager::manager<render_manager>::initialized = false;

/*!
  Generates the render_manager object
*/
inline render_manager::render_manager() : fps_counter(0), fps(0) {
    render_tmp_bmp = NULL;
    overlay_font = NULL;

    fps_timer = NULL;
    fps_event_queue = NULL;

    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    current_time = 0;
    #endif

    //  Define comparator as lambda function that sorts components
    render_comparator =
        [](entity_component_pair r_element1, entity_component_pair r_element2) {
            return r_element1.second < r_element2.second;
        };
}

/*!
  Cleans up the render_manager object
*/
inline render_manager::~render_manager() {
    al_destroy_bitmap(render_tmp_bmp);
    al_destroy_font(overlay_font);

    al_destroy_event_queue(fps_event_queue);
    al_destroy_timer(fps_timer);
}

/*!
  Pass an Allegro font for the render_manager to use
*/
inline void render_manager::initialize(ALLEGRO_FONT* font) {
    overlay_font = font;

    fps_timer = al_create_timer(1);

    fps_event_queue = al_create_event_queue();
    al_register_event_source(fps_event_queue, al_get_timer_event_source(fps_timer));

    al_start_timer(fps_timer);
}

/*!
  Gets passed the entity manager and timer then draws everything to screen
*/
inline void render_manager::render(menu_manager& menus, entity_manager& world) {
    bool queue_not_empty = false;

    //  Make sure we're always drawing to the screen
    al_set_target_backbuffer(al_get_current_display());
    al_clear_to_color(WTE_COLOR_BLACK);

    /*
      Calculate fps if enabled
    */
    if(engine_flags::is_set(DRAW_FPS)) {
        fps_counter++;
        //  Update fps on unique ticks only
        queue_not_empty = al_get_next_event(fps_event_queue, &fps_event);
        if(fps_event.type == ALLEGRO_EVENT_TIMER && queue_not_empty) {
            fps = fps_counter;
            fps_counter = 0;
        }
    }

    /*
      Render world if the game is running
    */
    if(engine_flags::is_set(GAME_STARTED)) {
        /*
          Draw the background
        */
        component_container layer_components = world.get_components<cmp::background_layer>();

        //  Sort the background layers
        std::set<entity_component_pair, comparator> layer_componenet_set(
            layer_components.begin(), layer_components.end(), render_comparator);

        //  Draw each background by layer
        for(ec_pair_iterator it = layer_componenet_set.begin(); it != layer_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible == true)
                al_draw_bitmap(world.get_component<cmp::background>(it->first)->background_bitmap, 0, 0, 0);
        }

        /*
          Draw the remaining entities
        */
        component_container render_components = world.get_components<cmp::render_order>();

        //  Sort the entity render components
        std::set<entity_component_pair, comparator> render_componenet_set(
            render_components.begin(), render_components.end(), render_comparator);

        //  Draw each entity in order
        for(ec_pair_iterator it = render_componenet_set.begin(); it != render_componenet_set.end(); it++) {
            if(world.get_component<cmp::visible>(it->first)->is_visible == true) {
                //  Draw...
                if(world.has_component<cmp::sprite>(it->first)) {
                    //
                }
                if(world.has_component<cmp::texture>(it->first)) {
                    //
                }
            }
        }

        /*
          Draw hitboxes if enabled
          Use different colors for each team
        */
        if(engine_flags::is_set(DRAW_HITBOX)) {
            for(ec_pair_iterator it = render_componenet_set.begin(); it != render_componenet_set.end(); it++) {
                //  Make sure the entity has a hitbox and is enabled
                if((world.has_component<cmp::hitbox>(it->first))
                &&
                (world.get_component<cmp::enabled>(it->first)->is_enabled == true)) {
                    //  Select color based on team
                    ALLEGRO_COLOR team_color;
                    switch(world.get_component<cmp::team>(it->first)->this_team) {
                        case 0: team_color = WTE_COLOR_GREEN; break;
                        case 1: team_color = WTE_COLOR_RED; break;
                        case 2: team_color = WTE_COLOR_BLUE; break;
                        default: team_color = WTE_COLOR_YELLOW;
                    }
                    //  Draw the hitbox
                    for(int i = 0; i < world.get_component<cmp::hitbox>(it->first)->width; i++) {
                        for(int j = 0; j < world.get_component<cmp::hitbox>(it->first)->height; j++) {
                            al_draw_pixel(world.get_component<cmp::location>(it->first)->pos_x + i,
                                        world.get_component<cmp::location>(it->first)->pos_y + j,
                                        team_color);
                        }
                    } //  End hitbox drawing
                } //  End hitbox/enabled test
            } //  End render component loop
        } //  End draw hitbox check
    } else {
        /*
          Game is not running - draw the title screen
        */
        //
    }

    /*
      Render game menu if it's opened
    */
    if(engine_flags::is_set(GAME_MENU_OPENED)) {
        render_tmp_bmp = al_clone_bitmap(menus.render_menu());
        al_set_target_backbuffer(al_get_current_display());
        al_draw_bitmap(render_tmp_bmp,
                       (wte_config::screen_width / 2) - (al_get_bitmap_width(render_tmp_bmp) / 2),
                       (wte_config::screen_height / 2) - (al_get_bitmap_height(render_tmp_bmp) / 2),
                       0);
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
      Render alerts
    */
    if(alert::is_set()) {
        int font_size = al_get_font_line_height(overlay_font);
        render_tmp_bmp = al_create_bitmap((alert::get_alert().length() * font_size) + 20, font_size + 20);
        al_set_target_bitmap(render_tmp_bmp);
        al_clear_to_color(alert::get_bg_color());

        al_draw_text(overlay_font, alert::get_font_color(),
                     (al_get_bitmap_width(render_tmp_bmp) / 2), 10,
                     ALLEGRO_ALIGN_CENTER, alert::get_alert().c_str());

        al_set_target_backbuffer(al_get_current_display());
        al_draw_bitmap(render_tmp_bmp,
                       (wte_config::screen_width / 2) - (al_get_bitmap_width(render_tmp_bmp) / 2),
                       (wte_config::screen_height / 2) - (al_get_bitmap_height(render_tmp_bmp) / 2),
                       0);
        al_destroy_bitmap(render_tmp_bmp);
    }

    /*
      Draw the overlay
    */
    std::string fps_string = "FPS: " + std::to_string(fps);
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    std::string timer_string = "Timer: " + std::to_string(current_time);
    #endif

    //  Draw frame rate
    if(engine_flags::is_set(DRAW_FPS)) {
        al_draw_text(overlay_font, WTE_COLOR_YELLOW, wte_config::screen_width, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }
    //  Draw time if debug mode is enabled
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    al_draw_text(overlay_font, WTE_COLOR_YELLOW, wte_config::screen_width, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
      Update the screen
    */
    if(engine_flags::is_set(WAIT_FOR_VSYNC)) al_wait_for_vsync();
    al_flip_display();
}

} //  namespace mgr

} //  namespace wte

#endif
