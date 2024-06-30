//
// Created by yanick on 24-6-30.
//

#ifndef KALEIDOSCOPE_IF_H
#define KALEIDOSCOPE_IF_H
#include "../llvm/llvm.h"
#include "ast.h"

/// IfExprAST - Expression class for if/then/else.
class IfExprAST : public ExprAST {
    std::unique_ptr<ExprAST> Cond, Then, Else;

    public:
    IfExprAST (std::unique_ptr<ExprAST> Cond,
    std::unique_ptr<ExprAST> Then,
    std::unique_ptr<ExprAST> Else)
    : Cond (std::move (Cond)), Then (std::move (Then)), Else (std::move (Else)) {
    }

    llvm::Value *codegen () override;
};

#endif // KALEIDOSCOPE_IF_H
