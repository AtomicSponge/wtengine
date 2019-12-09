/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_logic.hpp

  See LICENSE.txt for copyright information

  Header file for custom logic handler
*/

#ifndef WTEDEMO_CUSTOM_LOGIC_HPP
#define WTEDEMO_CUSTOM_LOGIC_HPP

#include "wte/wte.hpp"

class custom_logic : public wte::ecs::sys::logic {
    private:
        void custom_run(wte::ecs::entity_manager&, wte::msg::message_queue&, int64_t);
        void process_messages(wte::ecs::entity_manager&, wte::msg::message_container);
};

#endif
