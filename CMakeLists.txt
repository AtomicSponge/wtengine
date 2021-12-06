############################################################
#
#  WTEngine CMake
#
#  See LICENSE.md for copyright information.
#
#  This generates Makefiles for building the engine library.
#
############################################################

########################################
#
#  Project Configuration
#
########################################
#  Configure build project
cmake_minimum_required(VERSION 3.11)
project(wtengine VERSION 0.7 DESCRIPTION "WTEngine Game Engine")
enable_language(CXX)

#  If a build type is not set, set it to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#  Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

########################################
#
#  Library Detection
#
########################################
#  Check for PhysFS
find_library(PHYSFS_LIB NAMES physfs libphysfs)
if(NOT PHYSFS_LIB)
    message("Error! PhysFS not found!  Aborting.")
    return()
endif()

#  Check for Allegro and its required modules
find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED allegro-5)           #  Allegro base
    pkg_check_modules(ALLEGRO REQUIRED allegro_main-5)      #  Allegro main
    pkg_check_modules(ALLEGRO REQUIRED allegro_physfs-5)    #  PhysFS add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_audio-5)     #  Audio add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_font-5)      #  Font add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_image-5)     #  Image add-on

########################################
#
#  Configure Sources
#
########################################
add_library(wtengine STATIC 
    src/_globals/alert.cpp
    src/_globals/commands.cpp
    src/_globals/engine_time.cpp
    src/_globals/message.cpp
    src/_globals/wrappers.cpp
    src/cmp/ai.cpp
    src/cmp/background.cpp
    src/cmp/bounding_box.cpp
    src/cmp/dispatcher.cpp
    src/cmp/gfx.cpp
    src/cmp/hitbox.cpp
    src/cmp/location.cpp
    src/cmp/motion.cpp
    src/cmp/overlay.cpp
    src/cmp/sprite.cpp
    src/mgr/audio.cpp
    src/mgr/messages.cpp
    src/mgr/renderer.cpp
    src/mgr/spawner.cpp
    src/mgr/systems.cpp
    src/mgr/variables.cpp
    src/mgr/world.cpp
    src/sys/animate.cpp
    src/sys/collision.cpp
    src/sys/logic.cpp
    src/sys/movement.cpp
    src/config.cpp
    src/display.cpp
    src/input.cpp
    src/engine.cpp)

########################################
#
#  Build Process
#
########################################
#  Set include folder for building
target_include_directories(wtengine PRIVATE include)

#  Link libraries
target_link_libraries(wtengine PRIVATE ${ALLEGRO_LIBRARIES})

#  Set the build target
set_target_properties(wtengine PROPERTIES
    VERSION ${PROJECT_VERSION}
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED True)

########################################
#
#  Install Process
#
########################################
#  Get GNUInstallDirs
include(GNUInstallDirs)

#  Install the built library
install(TARGETS wtengine LIBRARY
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

#  Now install the headers, preserving subfolders
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp")

#  Create pkg-config and install
configure_file(wtengine.pc.in wtengine.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/wtengine.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#  Done!