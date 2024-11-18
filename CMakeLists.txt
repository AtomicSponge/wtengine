#  Silvergun CMake
#  See LICENSE.md for copyright information.

cmake_minimum_required(VERSION 3.11)
project(silvergun VERSION 0.8.0 DESCRIPTION "Silvergun Game Engine")
enable_language(CXX)

set(CMAKE_BUILD_TYPE Release)
include(GNUInstallDirs)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")
