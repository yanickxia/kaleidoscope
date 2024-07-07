//
// Created by yanick on 22-12-14.
//

#ifndef KALEIDOSCOPE_NUM_H
#define KALEIDOSCOPE_NUM_H

#include "../llvm/llvm.h"
#include "ast.h"


/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    llvm::Value* codegen() override;

    llvm::raw_ostream& dump(llvm::raw_ostream& out, int ind) override {
        return ExprAST::dump(out << Val, ind);
    }
};

#endif // KALEIDOSCOPE_NUM_H