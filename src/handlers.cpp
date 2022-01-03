/*!
 * WTEngine | File:  handlers.cpp
 * 
 * \author Matthew Evans
 * \version 0.71
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/handlers.hpp"

namespace wte {

bool handlers::initialized = false;

/*
 *
 */
handlers::handlers() {
    if(initialized) throw std::runtime_error("Handlers instance already running!");
    initialized = true;
}

/*
 *
 */
void handlers::run(const ALLEGRO_EVENT& event) {
    run_global(event);
    (config::flags::game_started ?
        run_game(event) :
        run_non_game(event));
}

}  //  end namespace wte
