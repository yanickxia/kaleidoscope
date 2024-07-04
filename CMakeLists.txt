cmake_minimum_required(VERSION 3.20.1)
project(kaleidoscope)


# package search for brew install packages
set(CMAKE_PREFIX_PATH "/usr/local/opt/llvm@14/" ${CMAKE_PREFIX_PATH})

find_package(LLVM 14 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

llvm_map_components_to_libnames(llvm_libs core orcjit native)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
        lexer/lexer.h lexer/lexer.cpp
        parser/parser.h parser/parser.cpp
        main.cpp
        ast/ast.h ast/num.cpp ast/num.h ast/variable.cpp ast/variable.h ast/binary.cpp ast/binary.h
        ast/call.cpp ast/call.h ast/prototype.cpp ast/prototype.h ast/function.cpp ast/function.h
        log/log.cpp log/log.h llvm/llvm.h kaleidoscope/kaleidoscope.cpp kaleidoscope/kaleidoscope.h
        jit/jit.h
        extern/extern.h
        extern/extern.h
        ast/if.h
        ast/if.cpp
        ast/for.cpp
        ast/for.h
        ast/unary.cpp
        ast/unary.h
        ast/var.cpp
        ast/var.h)

add_executable(kaleidoscope ${SOURCES})
target_link_libraries(kaleidoscope ${llvm_libs})
