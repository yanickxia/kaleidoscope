cmake_minimum_required(VERSION 3.24)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
    lexer/lexer.cpp
    parser/parser.cpp
    main.cpp)

add_executable(kaleidoscope ${SOURCES})
