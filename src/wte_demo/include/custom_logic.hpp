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

using namespace wte;

class custom_logic : public sys::logic {
    private:
        void custom_run(mgr::entity_manager&, mgr::message_manager&, int64_t);
        void process_message(mgr::entity_manager&, message);
};

#endif
