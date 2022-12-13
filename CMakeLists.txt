cmake_minimum_required(VERSION 3.24)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 17)

add_executable(kaleidoscope main.cpp lexer/lexer.h lexer/lexer.cpp lexer/token.h)
