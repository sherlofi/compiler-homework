cmake_minimum_required(VERSION 3.8)
project(compiler_homework)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES dr_compiler/lexer.cpp dr_compiler/lexer.h dr_compiler/main.cpp)
add_executable(compiler_homework ${SOURCE_FILES})