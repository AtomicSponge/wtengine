/*
  WTEngine Demo
  By:  Matthew Evans
  File:  custom_input.hpp

  See LICENSE.txt for copyright information

  Header file for custom input handler
*/

#ifndef WTEDEMO_CUSTOM_INPUT_HPP
#define WTEDEMO_CUSTOM_INPUT_HPP

#include <wte/wte.hpp>

using namespace wte;

class custom_input : public sys::input {
    private:
        void custom_run(mgr::entity_manager&, mgr::message_manager&);
};

#endif
