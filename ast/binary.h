//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_BINARY_H
#define KALEIDOSCOPE_BINARY_H

#include "ast.h"
#include <memory>

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

    public:
    BinaryExprAST (char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS)
    : Op (op), LHS (std::move (LHS)), RHS (std::move (RHS)) {
    }

    llvm::Value* codegen () override;
};

#endif // KALEIDOSCOPE_BINARY_H