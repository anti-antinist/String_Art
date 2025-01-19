cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 17)
project(Str_Art)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-15")
add_executable(Str_Art src/main.cpp src/PGM_HANDLER.cpp src/String_Art.cpp)

target_include_directories(Str_Art PUBLIC include/)
