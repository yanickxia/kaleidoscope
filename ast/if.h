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
    IfExprAST(
        SourceLocation Loc,
        std::unique_ptr<ExprAST> Cond,
        std::unique_ptr<ExprAST> Then,
        std::unique_ptr<ExprAST> Else)
        : ExprAST(Loc), Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}

    llvm::Value* codegen() override;

    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        ExprAST::dump(out << "if", ind);
        Cond->dump(indent(out, ind) << "Cond:", ind + 1);
        Then->dump(indent(out, ind) << "Then:", ind + 1);
        Else->dump(indent(out, ind) << "Else:", ind + 1);
        return out;
    }
};

#endif // KALEIDOSCOPE_IF_H