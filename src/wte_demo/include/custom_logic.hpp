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

class custom_logic : public wte::sys::logic {
    private:
        void custom_run(wte::mgr::entity_manager&, wte::mgr::message_manager&, int64_t);
        void process_message(wte::mgr::entity_manager&, wte::message);
};

#endif
