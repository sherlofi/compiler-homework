cmake_minimum_required(VERSION 3.8)
project(compiler_homework)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES dr_compiler/dr_lexical_analysis.cpp dr_compiler/dr_lexical_analysis.h dr_compiler/main.cpp)
add_executable(compiler_homework ${SOURCE_FILES})