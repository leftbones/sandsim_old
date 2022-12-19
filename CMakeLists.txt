cmake_minimum_required (VERSION 3.8)
project ("sandsim")

set(CMAKE_EXPORT_COMPILE_COMMANDS)

set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

set(PROJECT_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/sources/main.h"
"${CMAKE_CURRENT_SOURCE_DIR}/sources/element.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/sources/element.h"
"${CMAKE_CURRENT_SOURCE_DIR}/sources/menu.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/sources/menu.h"
)

add_executable (sandsim ${PROJECT_SOURCES})
target_link_libraries(sandsim PRIVATE raylib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sandsim PROPERTY CXX_STANDARD 20)
endif()