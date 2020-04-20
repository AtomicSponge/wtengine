cmake_minimum_required(VERSION 3.12.0 FATAL_ERROR)
project(wtengine VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include(CTest)
#enable_testing()

include_directories(${PROJECT_SOURCE_DIR})

#file(GLOB wtengine_SRC "*.cpp" CONFIGURE_DEPENDS)

#option(WTE_DEBUG_MODE)
#option(WTE_NO_AUDIO)
#option(WTE_MAX_SAMPLES)
#option(WTE_NO_PRUNE)

add_executable(wte_demo ${wtengine_SRC})

target_link_libraries(wte_demo allegro-5)

#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)
