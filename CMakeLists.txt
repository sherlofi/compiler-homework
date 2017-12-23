cmake_minimum_required(VERSION 3.8)
project(compiler_homework)

set(CMAKE_CXX_STANDARD 11)

set(
        SOURCE_FILES
        drawing-lang-compiler/lexer.cpp
        drawing-lang-compiler/lexer.hpp
#        drawing-lang-compiler/lexerMain.cpp
        drawing-lang-compiler/parser.hpp
        drawing-lang-compiler/parser.cpp
#        drawing-lang-compiler/parserMain.cpp
        drawing-lang-compiler/semantic.hpp
        drawing-lang-compiler/semantic.cpp
        drawing-lang-compiler/semanticMain.cpp
)
add_executable(compiler_homework ${SOURCE_FILES})