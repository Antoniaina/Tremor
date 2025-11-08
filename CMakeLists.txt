cmake_minimum_required(VERSION 3.10)
project(tremor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

include_directories(${PIPEWIRE_INCLUDE_DIRS})
link_directories(${PIPEWIRE_LIBRARY_DIRS})
add_definitions(${PIPEWIRE_CFLAGS_OTHER})

add_executable(tremor
    src/main.cpp
)

add_executable(list_nodes
  src/pipewireListNodes.cpp
)

target_link_libraries(tremor ${PIPEWIRE_LIBRARIES})
target_link_libraries(list_nodes ${PIPEWIRE_LIBRARIES})
