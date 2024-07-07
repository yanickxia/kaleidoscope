//
// Created by yanick on 24-7-1.
//

#ifndef KALEIDOSCOPE_UNARY_H
#define KALEIDOSCOPE_UNARY_H

#include "ast.h"

/// UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST : public ExprAST {
    char Opcode;
    std::unique_ptr<ExprAST> Operand;

public:
    UnaryExprAST(char Opcode, std::unique_ptr<ExprAST> Operand)
        : Opcode(Opcode), Operand(std::move(Operand)) {}

    llvm::Value* codegen() override;
    llvm::raw_ostream &dump(llvm::raw_ostream &out, int ind) override {
        ExprAST::dump(out << "unary" << Opcode, ind);
        Operand->dump(out, ind + 1);
        return out;
    }
};

#endif //KALEIDOSCOPE_UNARY_H