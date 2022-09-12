cmake_minimum_required(VERSION 3.0.0)

add_library(rdp_lib rdp.c)
target_include_directories(rdp_lib PUBLIC .)
