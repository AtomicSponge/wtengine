/*
  WTEngine
  By:  Matthew Evans
  File:  manager.hpp

  See LICENSE.txt for copyright information

  Manager
*/

#ifndef WTE_MGR_MANAGER_HPP
#define WTE_MGR_MANAGER_HPP

#include <stdexcept>

namespace wte
{

namespace mgr
{

template <typename derived> class manager {
    public:
        inline virtual ~manager() { initialized = false; };

        manager(const manager&) = delete;
        void operator=(manager const&) = delete;

    protected:
        static bool initialized;

        inline manager() {
            if(initialized == true) throw std::runtime_error("An instance of this manager is already running!");
            initialized = true;
        };
};

}  // end namespace mgr

}  // end namespace wte

#endif
