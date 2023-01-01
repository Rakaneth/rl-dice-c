cmake_minimum_required(VERSION 3.0.0)

project(rdp)

add_library(rdp_lib rdp.c)
target_include_directories(rl_dice PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
