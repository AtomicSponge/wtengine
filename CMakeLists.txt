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

#  Always build release
set(CMAKE_BUILD_TYPE Release)

#  Set compiler flags
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#  Set up flags for building with Emscripten
if(EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_FREETYPE=1 -s USE_VORBIS=1 -s USE_OGG=1 -s USE_LIBJPEG=1 -s USE_SDL=2 -s USE_LIBPNG=1 -O3")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_FREETYPE=1 -s USE_VORBIS=1 -s USE_OGG=1 -s USE_LIBJPEG=1 -s USE_SDL=2 -s USE_LIBPNG=1 -O3")
  set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
endif()

########################################
#
#  Library Detection
#
########################################
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
    #  Check for physfs
    pkg_check_modules(PHYSFS REQUIRED IMPORTED_TARGET physfs)
#  Check for OpenGL
find_package(OpenGL REQUIRED)

########################################
#
#  Install Process
#
########################################
#  Get GNUInstallDirs
include(GNUInstallDirs)

#  Now install the headers, preserving subfolders
install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

#  Create pkg-config and install
configure_file(wtengine.pc.in wtengine.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/wtengine.pc
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

#  Done!
