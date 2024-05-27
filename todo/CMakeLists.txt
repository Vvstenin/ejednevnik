cmake_minimum_required(VERSION 3.18)

set(project "todo")
project(${project})

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

add_executable(${project} main.cc)
target_link_libraries(${project} PRIVATE PkgConfig::GTK4)