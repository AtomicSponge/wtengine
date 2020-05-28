cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(wtengine VERSION 0.1.0)

find_package(PkgConfig REQUIRED)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

pkg_check_modules(alleg-5 REQUIRED IMPORTED_TARGET allegro-5)
pkg_check_modules(alleg_main-5 REQUIRED IMPORTED_TARGET allegro_main-5)
pkg_check_modules(alleg_physfs-5 REQUIRED IMPORTED_TARGET allegro_physfs-5)
pkg_check_modules(alleg_audio-5 REQUIRED IMPORTED_TARGET allegro_audio-5)
pkg_check_modules(alleg_acodec-5 REQUIRED IMPORTED_TARGET allegro_acodec-5)
pkg_check_modules(alleg_font-5 REQUIRED IMPORTED_TARGET allegro_font-5)
pkg_check_modules(alleg_image-5 REQUIRED IMPORTED_TARGET allegro_image-5)

pkg_check_modules(alleg_primitives-5 REQUIRED IMPORTED_TARGET allegro_primitives-5)

#include(CTest)
#enable_testing()

include_directories(${PROJECT_SOURCE_DIR})

#option(WTE_DEBUG_MODE)
#option(WTE_NO_AUDIO)
#option(WTE_MAX_SAMPLES)
#option(WTE_NO_PRUNE)

add_executable(wte_demo main.cpp
                        wte_demo.cpp
                        custom_input.cpp)

target_link_libraries(wte_demo libphysfs
                               pthread
                               PkgConfig::alleg-5
                               PkgConfig::alleg_main-5
                               PkgConfig::alleg_physfs-5
                               alleg_audio-5
                               alleg_acodec-5
                               alleg_font-5
                               alleg_image-5
                               alleg_primitives-5)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
