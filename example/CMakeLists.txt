############################################################
#
#  WTEngine Demo CMake
#
#  Ver:  032521
#  See LICENSE.md for copyright information.
#
#  This generates Makefiles for building the engine example.
#  Issue:  Not linking PhysFS??
#
############################################################

########################################
#
#  Project Configuration
#
########################################
#  Configure build project
cmake_minimum_required(VERSION 3.11)
project(wte_demo VERSION 1.0 DESCRIPTION "WTEngine Demo Game")
enable_language(CXX)

#  If a build type is not set, set it to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#  Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DWTE_DEBUG_MODE=9")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DWTE_DEBUG_MODE=0")

########################################
#
#  Library Detection
#
########################################
#  Set pthreading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#  Check for PhysFS
find_package(PhysFS REQUIRED)

#  pkg-config checks
find_package(PkgConfig REQUIRED)
    pkg_check_modules(_PKGCONFIG REQUIRED wtengine)             #  Check for WTEngine
    #  Check for Allegro and its required modules
    pkg_check_modules(_PKGCONFIG REQUIRED allegro-5)            #  Allegro base
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_main-5)       #  Allegro main
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_physfs-5)     #  PhysFS add-on
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_audio-5)      #  Audio add-on
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_acodec-5)     #  Audio codec add-on
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_font-5)       #  Font add-on
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_image-5)      #  Image add-on
    pkg_check_modules(_PKGCONFIG REQUIRED allegro_primitives-5) #  Primitives add-on

########################################
#
#  Configure Sources
#
########################################
add_executable(wte_demo
    src/main.cpp
    src/wte_demo.cpp)

########################################
#
#  Build Process
#
########################################
#  Link libraries
target_link_libraries(wte_demo
    Threads::Threads
    PhysFS
    ${_PKGCONFIG_LIBRARIES})

#  Set the build target
set_target_properties(wte_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED True)

#  No install for the demo - Done!
message("Might not work!!  Run build_example.sh instead")