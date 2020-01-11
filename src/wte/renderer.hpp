/*
  WTEngine
  By:  Matthew Evans
  File:  renderer.hpp

  See LICENSE.txt for copyright information

  Renderer object
*/

#ifndef WTE_RENDERER_HPP
#define WTE_RENDERER_HPP

#include <string>
#include <set>
#include <iterator>
#include <algorithm>
#include <functional>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "wte_globals.hpp"
#include "menu_manager.hpp"
#include "entity_manager.hpp"
#include "components\components.hpp"

namespace wte
{

//!  Define a container for an entity and component pair
typedef std::pair<ecs::entity, ecs::cmp::component_sptr> entity_component_pair;
//!  Iterator for the entity/component pair
typedef std::set<entity_component_pair>::iterator ec_pair_iterator;
//!  Comparator for sorting entity/component pairs
typedef std::function<bool(entity_component_pair, entity_component_pair)> comparator;

//! Renderer class
/*!
  An object that handles drawing the world to the screen
*/
class renderer {
    public:
        renderer();                           /*!< Basic constructor */
        renderer(ALLEGRO_FONT *);             /*!< Constructor to configure renderer */
        ~renderer();

        renderer(const renderer&) = delete;
        //void operator=(renderer const&) = delete;

        void render(mnu::menu_manager&, ecs::entity_manager&, int64_t);     /*!< Call the renderer */

    private:
        ALLEGRO_BITMAP *menu_bitmap;
        ALLEGRO_FONT *overlay_font;                     /*!< Allegro font used for the overlay */
        int64_t last_tick, this_tick;                   /*!< Store timer ticks to Calculate FPS */
        int fps_counter, fps;                           /*!< FPS counters */
        comparator render_comparator;                   /*!< Store lambda function for comparator */
};

//!
/*!
*/
inline renderer::renderer() {
    menu_bitmap = NULL;
    overlay_font = NULL;
}

//! Renderer constructor
/*!
  Call to create a new renderer object
  Gets passed an Allegro font to use for the overlay
*/
inline renderer::renderer(ALLEGRO_FONT *font) {
    last_tick = 0;
    this_tick = 0;
    fps_counter = 0;
    fps = 0;

    menu_bitmap = NULL;
    overlay_font = font;

    //  Define comparator as lambda function that sorts components
    render_comparator =
        [](entity_component_pair r_element1, entity_component_pair r_element2) {
            return r_element1.second < r_element2.second;
        };
}

//! Renderer destructor
/*!
  Cleans up the renderer object
*/
inline renderer::~renderer() {
    al_destroy_bitmap(menu_bitmap);
    //al_destroy_font(overlay_font);
}

//! Render method - Draw the game screen
/*!
  Gets passed the entity manager and timer then draws everything to screen
*/
inline void renderer::render(mnu::menu_manager& menus, ecs::entity_manager& world, int64_t current_time) {
    //  Make sure we're always drawing to the screen
    al_set_target_backbuffer(al_get_current_display());

    /*
      Calculate fps if enabled
    */
    if(game_flag[DRAW_FPS]) {
        fps_counter++;
        this_tick = current_time;
        //  Update fps on unique ticks only
        if(current_time % (int64_t)TICKS_PER_SECOND == 0 && this_tick != last_tick) {
            fps = fps_counter;
            fps_counter = 0;
            last_tick = current_time;
        }
    }

    /*
      Render world if the game is running
    */
    if(game_flag[GAME_STARTED]) {
        /*
          Draw the background
        */
        ecs::component_container layer_components = world.get_components<ecs::cmp::background_layer>();

        //  Sort the background layers
        std::set<entity_component_pair, comparator> layer_componenet_set(
            layer_components.begin(), layer_components.end(), render_comparator);

        //  Draw each background by layer
        for(ec_pair_iterator it = layer_componenet_set.begin(); it != layer_componenet_set.end(); it++) {
            if(world.get_component<ecs::cmp::visible>(it->first)->is_visible == true)
                al_draw_bitmap(world.get_component<ecs::cmp::background>(it->first)->background_bitmap, 0, 0, 0);
        }

        /*
          Draw the remaining entities
        */
        ecs::component_container render_components = world.get_components<ecs::cmp::render_order>();

        //  Sort the entity render components
        std::set<entity_component_pair, comparator> render_componenet_set(
            render_components.begin(), render_components.end(), render_comparator);

        //  Draw each entity in order
        for(ec_pair_iterator it = render_componenet_set.begin(); it != render_componenet_set.end(); it++) {
            if(world.get_component<ecs::cmp::visible>(it->first)->is_visible == true) {
                //  Draw...
                if(world.has_component<ecs::cmp::sprite>(it->first)) {
                    //
                }
                if(world.has_component<ecs::cmp::texture>(it->first)) {
                    //
                }
            }
        }

        /*
          Draw hitboxes if enabled
          Use different colors for each team
        */
        if(game_flag[DRAW_HITBOX]) {
            for(ec_pair_iterator it = render_componenet_set.begin(); it != render_componenet_set.end(); it++) {
                //  Make sure the entity has a hitbox and is enabled
                if((world.has_component<ecs::cmp::hitbox>(it->first))
                &&
                (world.get_component<ecs::cmp::enabled>(it->first)->is_enabled == true)) {
                    //  Select color based on team
                    ALLEGRO_COLOR team_color;
                    switch(world.get_component<ecs::cmp::team>(it->first)->team) {
                        case 0: team_color = WTE_COLOR_GREEN; break;
                        case 1: team_color = WTE_COLOR_RED; break;
                        case 2: team_color = WTE_COLOR_BLUE; break;
                        default: team_color = WTE_COLOR_YELLOW;
                    }
                    //  Draw the hitbox
                    for(int i = 0; i < world.get_component<ecs::cmp::hitbox>(it->first)->width; i++) {
                        for(int j = 0; j < world.get_component<ecs::cmp::hitbox>(it->first)->height; j++) {
                            al_draw_pixel(world.get_component<ecs::cmp::location>(it->first)->pos_x + i,
                                        world.get_component<ecs::cmp::location>(it->first)->pos_y + j,
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
    if(game_flag[GAME_MENU_OPENED]) {
        menu_bitmap = al_clone_bitmap(menus.render_menu());
        al_draw_bitmap(menu_bitmap,
                       (WTE_ARENA_WIDTH / 2) - (al_get_bitmap_width(menu_bitmap) / 2),
                       (WTE_ARENA_HEIGHT / 2) - (al_get_bitmap_height(menu_bitmap) / 2),
                       0);
        al_destroy_bitmap(menu_bitmap);
    }

    /*
      Draw the overlay
    */
    std::string fps_string = "FPS: " + std::to_string(fps);
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    std::string timer_string = "Timer: " + std::to_string(current_time);
    #endif

    //  Draw frame rate
    if(game_flag[DRAW_FPS]) {
        al_draw_text(overlay_font, WTE_COLOR_WHITE, WTE_ARENA_WIDTH, 1, ALLEGRO_ALIGN_RIGHT, fps_string.c_str());
    }
    //  Draw time if debug mode is enabled
    #if WTE_DEBUG_MODE == 1 || WTE_DEBUG_MODE == 9
    al_draw_text(overlay_font, WTE_COLOR_WHITE, WTE_ARENA_WIDTH, 10, ALLEGRO_ALIGN_RIGHT, timer_string.c_str());
    #endif

    /*
      Update the screen
    */
    al_flip_display();
}

} //  namespace wte

#endif
