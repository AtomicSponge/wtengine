/*
  WTEngine
  By:  Matthew Evans
  File:  name.hpp

  See LICENSE.txt for copyright information

  Name component
*/

#ifndef WTE_CMP_NAME_HPP
#define WTE_CMP_NAME_HPP

#include <string>

#include "component.hpp"

namespace wte
{

namespace cmp
{

//! Name component
/*!
  Store the name of an entity
*/
class name final : public component {
    public:
        inline name(std::string n) : name_str(n) {};

        std::string name_str;
};

} //  namespace cmp

} //  namespace wte

#endif
