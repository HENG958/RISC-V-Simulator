cmake_minimum_required(VERSION 3.16)
project(CPU)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -g )


set(src_dir
  src/parser.cpp
  src/memory.cpp
  src/main.cpp
# std.cpp
)

add_executable(code ${src_dir})