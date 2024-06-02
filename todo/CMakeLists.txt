cmake_minimum_required(VERSION 3.10)
project(DiaryApp)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(DiaryApp main.cpp DiaryWindow.cpp DiaryData.cpp utils/FileUtils.cpp)

target_link_libraries(DiaryApp ${GTK_LIBRARIES})
