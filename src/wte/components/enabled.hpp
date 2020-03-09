/*
  WTEngine
  By:  Matthew Evans
  File:  enabled.hpp

  See LICENSE.txt for copyright information

  Enabled component
*/

#ifndef WTE_CMP_ENABLED_HPP
#define WTE_CMP_ENABLED_HPP

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Enabled component
/*!
  Toggle an entity to enabled or disabled state
*/
class enabled final : public component {
    public:
        inline enabled() : is_enabled(true) {};
        inline enabled(bool e) : is_enabled(e) {};

        bool is_enabled;
};

} //  namespace cmp

} //  namespace wte

#endif
