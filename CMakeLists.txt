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
project(wtengine VERSION 0.7.3 DESCRIPTION "What the Engine?")
enable_language(CXX)

#  If a build type is not set, set it to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#  Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#  Set up flags for building with Emscripten
if(EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
endif()

#  Set build variables
if(DEFINED WTE_REQUIRE_OPENGL_LATEST)
    list(APPEND WTE_BUILD_LIST "-DWTE_REQUIRE_OPENGL_LATEST")
endif()
#if(DEFINED WTE_BUILD_DEBUG)
    list(APPEND WTE_BUILD_LIST "-DWTE_BUILD_DEBUG")
#endif()
if(DEFINED WTE_DISABLE_KEYBOARD)
    list(APPEND WTE_BUILD_LIST "-DWTE_DISABLE_KEYBOARD")
endif()
if(DEFINED WTE_DISABLE_MOUSE)
    list(APPEND WTE_BUILD_LIST "-DWTE_DISABLE_MOUSE")
endif()
if(DEFINED WTE_DISABLE_JOYSTICK)
    list(APPEND WTE_BUILD_LIST "-DWTE_DISABLE_JOYSTICK")
endif()
if(DEFINED WTE_DISABLE_TOUCH)
    list(APPEND WTE_BUILD_LIST "-DWTE_DISABLE_TOUCH")
endif()

#  Set tick rate if defined
if(DEFINED WTE_TICKS_PER_SECOND)
    list(APPEND WTE_BUILD_LIST "-DWTE_TICKS_PER_SECOND=${WTE_TICKS_PER_SECOND}")
endif(DEFINED WTE_TICKS_PER_SECOND)

#  Set max samples if defined
if(DEFINED WTE_MAX_PLAYING_SAMPLES)
    list(APPEND WTE_BUILD_LIST "-DWTE_MAX_PLAYING_SAMPLES=${WTE_MAX_PLAYING_SAMPLES}")
endif(DEFINED WTE_MAX_PLAYING_SAMPLES)

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

#  Check for OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

########################################
#
#  Configure Sources
#
########################################
add_library(wtengine STATIC 
    src/_debug/exceptions.cpp    
    src/_debug/logger.cpp
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
    src/engine.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_impl_allegro5.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/imgui.cpp)

########################################
#
#  Build Process
#
########################################
#  Set include folder for building
target_include_directories(wtengine PRIVATE include ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

#  Link libraries
target_link_libraries(wtengine PRIVATE ${ALLEGRO_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

#  Set complie options
target_compile_options(wtengine PRIVATE ${WTE_BUILD_LIST})

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
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

#  Create pkg-config and install
configure_file(wtengine.pc.in wtengine.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/wtengine.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#  Done!