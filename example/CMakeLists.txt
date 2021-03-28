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
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DWTE_DEBUG_MODE=0 -lphysfs")

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
    #  Check for Allegro and its required modules
    pkg_check_modules(ALLEGRO REQUIRED IMPORTED_TARGET allegro-5)
    pkg_check_modules(ALLEGRO_MAIN REQUIRED IMPORTED_TARGET allegro_main-5)
    pkg_check_modules(ALLEGRO_PHYSFS REQUIRED IMPORTED_TARGET allegro_physfs-5)
    pkg_check_modules(ALLEGRO_AUDIO REQUIRED IMPORTED_TARGET allegro_audio-5)
    pkg_check_modules(ALLEGRO_ACODEC REQUIRED IMPORTED_TARGET allegro_acodec-5)
    pkg_check_modules(ALLEGRO_FONT REQUIRED IMPORTED_TARGET allegro_font-5)
    pkg_check_modules(ALLEGRO_IMAGE REQUIRED IMPORTED_TARGET allegro_image-5)
    pkg_check_modules(ALLEGRO_PRIMV REQUIRED IMPORTED_TARGET allegro_primitives-5)
    #  Check for WTEngine
    pkg_check_modules(WTENGINE REQUIRED IMPORTED_TARGET wtengine)

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
target_link_libraries(wte_demo PUBLIC
    Threads::Threads
    #PhysFS
    PkgConfig::WTENGINE
    PkgConfig::ALLEGRO
    PkgConfig::ALLEGRO_MAIN
    PkgConfig::ALLEGRO_PHYSFS
    PkgConfig::ALLEGRO_AUDIO
    PkgConfig::ALLEGRO_ACODEC
    PkgConfig::ALLEGRO_FONT
    PkgConfig::ALLEGRO_IMAGE
    PkgConfig::ALLEGRO_PRIMV)

#  Set the build target
set_target_properties(wte_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED True)

#  No install for the demo - Done!