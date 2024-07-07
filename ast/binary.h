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
    BinaryExprAST(
        SourceLocation Loc,
        char op,
        std::unique_ptr<ExprAST> LHS,
        std::unique_ptr<ExprAST> RHS)
        : ExprAST(Loc), Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

    llvm::Value* codegen() override;

    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        ExprAST::dump(out << "binary" << Op, ind);
        LHS->dump(indent(out, ind) << "LHS:", ind + 1);
        RHS->dump(indent(out, ind) << "RHS:", ind + 1);
        return out;
    }
};

#endif // KALEIDOSCOPE_BINARY_H