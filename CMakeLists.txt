cmake_minimum_required(VERSION 3.24)
project(kaleidoscope)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
        lexer/lexer.h lexer/lexer.cpp
        parser/parser.h parser/parser.cpp
        main.cpp
        ast/ast.h ast/num.cpp ast/num.h ast/variable.cpp ast/variable.h ast/binary.cpp ast/binary.h
        ast/call.cpp ast/call.h ast/prototype.cpp ast/prototype.h ast/function.cpp ast/function.h
        log/log.cpp log/log.h)

add_executable(kaleidoscope ${SOURCES})
