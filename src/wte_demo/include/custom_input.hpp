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

class custom_input : public wte::sys::input {
    private:
        void custom_run(wte::mgr::entity_manager&, wte::mgr::message_manager&);
};

#endif
