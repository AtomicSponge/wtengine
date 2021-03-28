############################################################
#
#  WTEngine CMake
#
#  Ver:  032821
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
project(wtengine VERSION 0.2 DESCRIPTION "WTEngine Game Engine")
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
#find_package(PhysFS REQUIRED)

#  Check for Allegro and its required modules
find_package(PkgConfig REQUIRED)
    pkg_check_modules(ALLEGRO REQUIRED allegro-5)           #  Allegro base
    pkg_check_modules(ALLEGRO REQUIRED allegro_main-5)      #  Allegro main
    pkg_check_modules(ALLEGRO REQUIRED allegro_physfs-5)    #  PhysFS add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_audio-5)     #  Audio add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_acodec-5)    #  Audio codec add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_font-5)      #  Font add-on
    pkg_check_modules(ALLEGRO REQUIRED allegro_image-5)     #  Image add-on

########################################
#
#  Configure Sources
#
########################################
add_library(wtengine STATIC
    src/wte_display.cpp
    src/wte_input.cpp
    src/cmp/ai.cpp
    src/cmp/bounding_box.cpp
    src/cmp/direction.cpp
    src/cmp/dispatcher.cpp
    src/cmp/enabled.cpp
    src/cmp/hitbox.cpp
    src/cmp/input_button.cpp
    src/cmp/input_directional.cpp
    src/cmp/location.cpp
    src/cmp/sample_loop.cpp
    src/cmp/team.cpp
    src/cmp/velocity.cpp
    src/cmp/visible.cpp
    src/cmp/gfx/animator.cpp
    src/cmp/gfx/background.cpp
    src/cmp/gfx/bitmap_map.cpp
    src/cmp/gfx/overlay.cpp
    src/cmp/gfx/sprite.cpp
    src/mgr/audio_manager.cpp
    src/mgr/message_manager.cpp
    src/mgr/render_manager.cpp
    src/mgr/spawn_manager.cpp
    src/mgr/system_manager.cpp
    src/mnu/menu_item_action.cpp
    src/mnu/menu_item_apply.cpp
    src/mnu/menu_item_selection.cpp
    src/mnu/menu_item_toggle.cpp
    src/sys/animate.cpp
    src/sys/collision.cpp
    src/sys/input.cpp
    src/sys/logic.cpp
    src/sys/movement.cpp)

########################################
#
#  Build Process
#
########################################
#  Set include folder for building
target_include_directories(wtengine PRIVATE include)

#  Link libraries
target_link_libraries(wtengine PRIVATE
    Threads::Threads
    #PhysFS
    ${ALLEGRO_LIBRARIES})

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