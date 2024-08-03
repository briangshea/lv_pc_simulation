cmake_minimum_required(VERSION 3.10)

project(widgets)

set(WIDGETS_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES ${WIDGETS_ROOT_DIR}/*.c)

add_library(widgets ${SOURCES})
target_link_libraries(widgets lvgl)
target_include_directories(widgets SYSTEM PUBLIC ${WIDGETS_ROOT_DIR})
