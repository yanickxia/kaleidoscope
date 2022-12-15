//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include "../llvm/llvm.h"

/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
    virtual ~ExprAST() = default;
    virtual llvm::Value *codegen() = 0;
};

#endif //KALEIDOSCOPE_AST_H
