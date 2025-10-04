cmake_minimum_required(VERSION 3.30.0)

project(rdp)

add_library(rl_dice rdp.c)
target_include_directories(rl_dice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
