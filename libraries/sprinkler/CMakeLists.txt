cmake_minimum_required(VERSION 3.10)
project(sprinkler)

set(SPRINKLER_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES ${SPRINKLER_ROOT_DIR}/src/*.c)

add_library(sprinkler ${SOURCES} )

target_include_directories(sprinkler SYSTEM PUBLIC ${SPRINKLER_ROOT_DIR}/include)
