//
// Created by yanick on 24-6-30.
//

#ifndef KALEIDOSCOPE_FOR_H
#define KALEIDOSCOPE_FOR_H

#include "ast.h"

/// ForExprAST - Expression class for for/in.
class ForExprAST : public ExprAST {
    std::string VarName;
    std::unique_ptr<ExprAST> Start, End, Step, Body;

    public:
    ForExprAST (const std::string &VarName,
    std::unique_ptr<ExprAST> Start,
    std::unique_ptr<ExprAST> End,
    std::unique_ptr<ExprAST> Step,
    std::unique_ptr<ExprAST> Body)
    : VarName (VarName), Start (std::move (Start)), End (std::move (End)),
      Step (std::move (Step)), Body (std::move (Body)) {
    }

    llvm::Value *codegen () override;
};

#endif // KALEIDOSCOPE_FOR_H
