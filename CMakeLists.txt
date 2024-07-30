cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++2a -g )


set(src_dir
#   src/parser.cpp
#   src/memory.cpp
#   src/tomasulo.cpp
  src/parser.cpp
  src/alu.cpp
  src/memory.cpp
  src/tomasulo.cpp
  src/main.cpp
)

add_executable(code ${src_dir})