/*
  WTEngine Demo
  By:  Matthew Evans
  File:  starfield.hpp

  See LICENSE.txt for copyright information

  Header file for the starfield system
*/

#ifndef WTEDEMO_SYSTEM_STARFIELD_HPP
#define WTEDEMO_SYSTEM_STARFIELD_HPP

#include "wte/wte.hpp"

#define MAX_STARS 64

class starfield : public wte::ecs::sys::system {
    public:
        void run(wte::ecs::entity_manager&, wte::msg::message_queue&, int64_t);
        void dispatch(wte::ecs::entity_manager&, wte::msg::message_queue&);

        starfield();
    private:
        float x[MAX_STARS];
        float y[MAX_STARS];
        float speed[MAX_STARS];
        float color[MAX_STARS];

        float speed_mult;
    
};

#endif
