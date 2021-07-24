/*!
 * WTEngine | File:  interface.cpp
 * 
 * \author Matthew Evans
 * \version 0.5
 * \copyright See LICENSE.md for copyright information.
 * \date 2019-2021
 */

#include "wtengine/mgr/interface.hpp"

namespace wte
{

namespace mgr
{

template <> bool interface::manager<interface>::initialized = false;

/*
 *
 */
void interface::audio_init(void) { mgr::audio::initialize(); }

} //  end namespace mgr

} //  end namespace wte
