/*!
 * WTEngine | File:  menus.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/menus.hpp"

namespace wte
{

namespace mgr
{

template <> bool menus::manager<menus>::initialized = false;

ALLEGRO_TIMER* menus::menu_timer = NULL;
ALLEGRO_EVENT_QUEUE* menus::menu_event_queue = NULL;
ALLEGRO_COLOR menus::menu_font_color;
wte_asset<al_bitmap> menus::menu_buffer;
wte_asset<al_bitmap> menus::menu_temp_bmp;
wte_asset<al_bitmap> menus::menu_background;
wte_asset<al_font> menus::menu_font;
wte_asset<al_bitmap> menus::cursor_bitmap;
std::vector<mnu::menu_sptr> menus::_menus;
std::stack<mnu::menu_csptr> menus::opened_menus;
mnu::menu_item_citerator menus::menu_position;
int menus::font_size = 0;
int menus::menu_padding = 0;
int menus::menu_width = 0;
int menus::menu_height = 0;
bool menus::select_menu_option = false;
bool menus::is_button_left = true;
int64_t menus::last_tick = 0;

/*
 *
 */
void menus::initialize(void) {
    //  Create the main menu.
    if(!new_menu(mnu::menu("main_menu", "Main Menu")))
        throw std::runtime_error("Unable to create main menu!");
    //  Create the in-game menu.
    if(!new_menu(mnu::menu("game_menu", "Game Menu")))
        throw std::runtime_error("Unable to create game menu!");

    //  Create the the menu bitmap for rendering.
    menu_buffer = make_asset(al_bitmap(
        mgr::renderer::get_arena_width(),
        mgr::renderer::get_arena_height(),
        true
    ));

    //  Create timer & its queue.
    menu_timer = al_create_timer(1.0f / 30.0f);
    if(!menu_timer) throw std::runtime_error("Failed to create menu timer!");
    menu_event_queue = al_create_event_queue();
    if(!menu_event_queue) throw std::runtime_error("Failed to create menu event queue!");
    al_register_event_source(menu_event_queue, al_get_timer_event_source(menu_timer));
    al_set_timer_count(menu_timer, 0);
}

/*
 *
 */
void menus::de_init(void) {
    al_destroy_event_queue(menu_event_queue);
    al_destroy_timer(menu_timer);
}

/*
 *
 */
void menus::set_background(wte_asset<al_bitmap> bmp) {
    menu_background = bmp;
    menu_temp_bmp = bmp;
    menu_width = al_get_bitmap_width(**menu_background);
    menu_height = al_get_bitmap_height(**menu_background);
}

/*
 *
 */
void menus::set_padding(const int& p) { menu_padding = p; }

/*
 *
 */
void menus::set_font(
    wte_asset<al_font> font,
    const ALLEGRO_COLOR& fcolor
) {
    menu_font = font;
    menu_font_color = fcolor;
    font_size = al_get_font_line_height(**menu_font);
}

/*
 *
 */
void menus::set_cursor(wte_asset<al_bitmap> bmp) { cursor_bitmap = bmp; }

/*
 *
 */
const bool menus::new_menu(const mnu::menu& new_menu) {
    for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
        if(new_menu.get_id() == (*it)->get_id()) return false;
    }
    _menus.push_back(std::make_shared<mnu::menu>(new_menu));
    return true;
}

/*
 *
 */
const mnu::menu_csptr menus::get_menu(const std::string& name) {
    if(_menus.empty()) throw std::runtime_error("No menus have been loaded!");

    for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }

    //  Menu not found, return main menu or game menu if the game is running
    if(config::flags::game_started) {
        for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
            if("game_menu" == (*it)->get_id()) return *it;
        }
    } else {
        for(menu_citerator it = _menus.begin(); it != _menus.end(); it++) {
            if("main_menu" == (*it)->get_id()) return *it;
        }
    }

    //  Menu still not found - just return the first one in the vector
    return *_menus.begin();
}

/*
 *
 */
const mnu::menu_sptr menus::set_menu(const std::string& name) {
    if(_menus.empty()) throw std::runtime_error("No menus have been loaded!");

    for(menu_iterator it = _menus.begin(); it != _menus.end(); it++) {
        if(name == (*it)->get_id()) return *it;
    }

    //  Menu not found - just return the first one in the vector
    return *_menus.begin();
}

/*
 *
 */
void menus::reset(void) {
    opened_menus = {};
    config::flags::game_menu_opened = false;
}

/*
 *
 */
void menus::open_menu(const std::string& menu_id) {
    opened_menus.push(get_menu(menu_id));
    config::flags::game_menu_opened = true;
    menu_position = opened_menus.top()->items_cbegin();

    //  Set default values for any menu settings objects.
    for(auto it = opened_menus.top()->items_cbegin(); it != opened_menus.top()->items_cend(); it++) {
        (*it)->set_default();
    }
}

/*
 *
 */
void menus::close_menu(void) {
    al_stop_timer(menu_timer);
    al_set_timer_count(menu_timer, 0);
    opened_menus.pop();
    if(opened_menus.empty()) config::flags::game_menu_opened = false;
    else menu_position = opened_menus.top()->items_cbegin();
}

/*
 *
 */
void menus::menu_pos_up(void) {
    if(menu_position != opened_menus.top()->items_cbegin()) menu_position--;
}

/*
 *
 */
void menus::menu_pos_down(void) {
    if(menu_position != --opened_menus.top()->items_cend()) menu_position++;
}

/*
 *
 */
void menus::menu_pos_start_left(void) {
    if(menu_position != opened_menus.top()->items_cend()) {
        is_button_left = true;
        select_menu_option = true;
        last_tick = al_get_timer_count(menu_timer);
    }
}

/*
 *
 */
void menus::menu_pos_stop_left(void) { select_menu_option = false; }

/*
 *
 */
void menus::menu_pos_start_right(void) {
    if(menu_position != opened_menus.top()->items_cend()) {
        is_button_left = false;
        select_menu_option = true;
        last_tick = al_get_timer_count(menu_timer);
    }
}

/*
 *
 */
void menus::menu_pos_stop_right(void) { select_menu_option = false; }

/*
 *
 */
void menus::menu_item_select(void) {
    if(menu_position != opened_menus.top()->items_cend()) (*menu_position)->on_select();
}

/*
 *
 */
void menus::run(void) {
    if(opened_menus.empty()) {
        //  No menus currently opened, add one to the stack.
        if(config::flags::game_started) open_menu("game_menu");  //  Add the in-game menu to the stack.
        else open_menu("main_menu");  //  Add the main menu to the stack.
    }
    if(!al_get_timer_started(menu_timer)) al_resume_timer(menu_timer);  //  Make sure timer is running.

    ALLEGRO_EVENT event;
    const bool queue_not_empty = al_get_next_event(menu_event_queue, &event);
    if(queue_not_empty && event.type == ALLEGRO_EVENT_TIMER) {
        if(select_menu_option) {
            bool toggle_menu_item = false;

                if(al_get_timer_count(menu_timer) >= last_tick + 60) toggle_menu_item = true;
                else {
                    if(al_get_timer_count(menu_timer) >= last_tick + 30) {
                        if(al_get_timer_count(menu_timer) % 2 == 0) toggle_menu_item = true;
                    } else {
                        if(al_get_timer_count(menu_timer) == last_tick + 15) toggle_menu_item = true;
                        else if(al_get_timer_count(menu_timer) == last_tick + 1) toggle_menu_item = true;
                    }
                }

            if(toggle_menu_item) {
                if(is_button_left) (*menu_position)->on_left();
                else (*menu_position)->on_right();
            }
        }
    }
}

/*
 *
 */
ALLEGRO_BITMAP* menus::render_menu(void) {
    //  Clear the menu buffer.
    al_set_target_bitmap(**menu_buffer);
    al_clear_to_color(WTE_COLOR_TRANSPARENT);

    //  Set drawing to the menu bitmap.
    al_set_target_bitmap(**menu_temp_bmp);
    al_draw_bitmap(**menu_background, 0, 0, 0);
    //  Render menu title.
    al_draw_text(
        **menu_font, menu_font_color,
        menu_width / 2, menu_padding,
        ALLEGRO_ALIGN_CENTER,
        opened_menus.top()->get_title().c_str()
    );

    //  Render menu items.
    float cursor_pos = 10.0;
    float vpart = 0.0, hpart = 0.0, offset = 0.0;
    std::size_t vcounter = 0;

    offset = menu_padding + font_size + menu_padding;
    vpart = (menu_height - offset) / (opened_menus.top()->num_items() + 1);
    for(auto it = opened_menus.top()->items_cbegin(); it != opened_menus.top()->items_cend(); it++) {
        vcounter++;
        hpart = menu_width / ((*it)->get_text().size() + 1);
        for(std::size_t i = 0; i < (*it)->get_text().size(); i++)
            al_draw_text(
                **menu_font, menu_font_color,
                hpart * (i + 1), (offset / 2) + (vpart * vcounter),
                ALLEGRO_ALIGN_CENTER,
                (*it)->get_text()[i].c_str()
            );
        if(it == menu_position) cursor_pos = (offset / 2) + (vpart * vcounter);
    }

    //  Render menu cursor.
    if(opened_menus.top()->num_items() != 0) al_draw_bitmap(**cursor_bitmap, menu_padding, cursor_pos, 0);

    //  Draw rendered menu.
    al_set_target_bitmap(**menu_buffer);
    al_draw_scaled_bitmap(
        **menu_temp_bmp, 0, 0, menu_width, menu_height,
        (mgr::renderer::get_arena_width() / 2) - (menu_width * config::gfx::menu_scale_factor / 2),
        (mgr::renderer::get_arena_height() / 2) - (menu_height * config::gfx::menu_scale_factor / 2),
        menu_width * config::gfx::menu_scale_factor,
        menu_height * config::gfx::menu_scale_factor, 0
    );

    return **menu_buffer;
}

}  // end namespace mgr

}  // end namespace wte
